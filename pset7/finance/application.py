from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
from datetime import datetime

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    # get the current cash
    cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])[0]["cash"]
    # total money
    total = cash
    # get the current shares
    categorize = db.execute("SELECT * FROM shares WHERE user_id = :id", id=session["user_id"])

    # iterate through to find the current value of the shares and calculate the total money
    try:
        for i in range(len(categorize)):
            if len(categorize) != 0:
                categorize[i]["price"] = lookup(categorize[i]["symbol"])["price"]
                categorize[i]["total"] = categorize[i]["price"] * categorize[i]["shares"]
                total += categorize[i]["total"]
    except:
        return apology("Internal server error")
    return render_template("index.html", total = round(total, 2), shares=categorize, cash = round(cash, 2))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "GET":
        return render_template("buy.html", bought = False)
    elif request.method == "POST":
        # try:
        symbol = request.form.get("symbol")
        print(symbol)
        print(lookup("NFLX"))
        infos = lookup(symbol.strip())
        print(request.form.get("shares"))
        print(infos)

        shares = int(request.form.get("shares"))
        db.execute("INSERT INTO 'transaction' (user_id, symbol, shares, price, time) VALUES (:user_id, :symbol, :shares, :price, :time)",
                    user_id = session["user_id"], symbol = infos["symbol"], shares = shares, price = infos["price"],
                    time = datetime.now())
        if db.execute("SELECT * FROM shares WHERE user_id = :id AND symbol = :symbol", id=session["user_id"],
                        symbol = infos["symbol"]):
            db.execute("UPDATE shares SET shares = shares + :shares WHERE user_id = :user_id AND symbol = :symbol",
                        shares=shares, user_id=session["user_id"], symbol=infos["symbol"])
        else:
            db.execute("INSERT INTO shares (user_id, symbol, name, shares) VALUES (:user_id, :symbol, :name, :shares)",
                        user_id = session["user_id"], symbol = infos["symbol"], name = infos["name"], shares = shares)
        db.execute("UPDATE users SET cash = cash - :cash WHERE id = :id", cash = shares * infos["price"], id=session["user_id"])
        return render_template("buy.html", bought = True)

        # except:
        #     return apology("Symbol not valid")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    return render_template("history.html", shares = db.execute("SELECT * FROM 'transaction' WHERE user_id = :user_id",
                            user_id = session["user_id"]))

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    elif request.method == "POST":
        try:
            price = lookup(request.form.get("symbol"))["price"]
            return render_template("quoted.html", symbol = request.form.get("symbol"), price = price)
        except:
            return apology("Symbol is not valid")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "GET":
        return render_template("register.html")
    elif request.method == "POST":
        # check if the username exists
        rows = db.execute("SELECT username FROM users")
        if not request.form.get("username") or not request.form.get("password") or not request.form.get("repassword"):
            return apology("INVALID USERNAME OR PASSWORD")
        elif request.form.get("username") in rows:
            return apology("USERNAME TAKEN")
        elif request.form.get("password") != request.form.get("repassword"):
            return apology("PASSWORDS DO NOT MATCH")
        else:
            db.execute("INSERT INTO users(username, hash) VALUES (:username, :hash)", username=request.form.get("username"),
                        hash = pwd_context.hash(request.form.get("password")))
            return redirect(url_for("login"))
    return apology("TODO")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "GET":
        rows = db.execute("SELECT symbol FROM shares WHERE user_id = :user_id GROUP BY symbol", user_id = session["user_id"])
        print(rows)
        return render_template("sell.html", shares = rows, sold=False)
    if request.method == "POST":
        print(request.form.get("symbol"))
        print(request.form.get("shares"))
        current_shares = db.execute("SELECT shares FROM shares WHERE user_id = :user_id and symbol = :symbol", user_id = session["user_id"],
                            symbol = request.form.get("symbol"))[0]["shares"]
        shares = current_shares - int(request.form.get("shares"))
        cash = float(db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session["user_id"])[0]["cash"])
        if shares <= 0:
            db.execute("DELETE FROM shares WHERE user_id = :user_id AND symbol = :symbol", user_id = session["user_id"],
                        symbol = request.form.get("symbol"))
            cash = cash + lookup(request.form.get("symbol"))["price"] * int(current_shares)
        else:
            db.execute("UPDATE shares SET shares = :shares WHERE user_id = :user_id AND symbol = :symbol", shares = shares,
                        user_id = session["user_id"], symbol = request.form.get("symbol"))
            cash = cash + lookup(request.form.get("symbol"))["price"] * int(request.form.get("shares"))


        db.execute("UPDATE users SET cash = :cash WHERE id = :id", id = session["user_id"], cash = cash)
        rows = db.execute("SELECT symbol FROM shares WHERE user_id = :user_id GROUP BY symbol", user_id = session["user_id"])
        return render_template("sell.html", shares = rows, sold=True)

@app.route("/change", methods=["GET", "POST"])
@login_required
def password_change():
    """Change password of the user."""
    if request.method == "GET":
        return render_template("change.html", changed = False)
    if request.method == "POST":
        if request.form.get("password") != request.form.get("repassword"):
            return apology("Passwords do not match")
        elif not request.form.get("password") or not request.form.get("repassword"):
            return apology("Please fill in all the space.")
        db.execute("UPDATE users SET hash = :hash WHERE id = :user_id", hash = pwd_context.hash(request.form.get("password")),
                    user_id = session["user_id"])
        return render_template("change.html", changed = True)
