from flask import Flask, redirect, render_template, request, url_for

import helpers
from analyzer import Analyzer
import os
import sys

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name, count=100)

    # get analyzer object and set counter to 0
    positive, negative, neutral = 0.0, 0.0, 0.0
    analyzer = Analyzer(os.path.join(sys.path[0], "positive-words.txt"),
            os.path.join(sys.path[0], "negative-words.txt"))

    # iterate through all the tweets
    for i in tweets:
        # calculate score and then add to counter
        if analyzer.analyze(i) > 0.0:
            positive += 1.0
        elif analyzer.analyze(i) < 0.0:
            negative += 1.0
        else:
            neutral += 1.0
    # map all to percentage
    positive = (positive / (positive + negative + neutral)) * 100.0
    negative = (negative / (positive + negative + neutral)) * 100.0
    neutral = (neutral / (positive + negative + neutral)) * 100.0


    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
