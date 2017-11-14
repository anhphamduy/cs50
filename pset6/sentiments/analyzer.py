import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        # get token object
        self.tokenizer = nltk.tokenize.TweetTokenizer()
        # lists to store words
        self.positives = []
        self.negatives = []

        # open file and then add words to list
        with open(positives, "r") as f:
            for i in f:
                if not i.startswith(";"):
                    self.positives.append(i.strip())
        with open(negatives, "r") as f:
            for i in f:
                if not i.startswith(";"):
                    i = i.rstrip()
                    self.negatives.append(i.strip())


    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score = 0
        # iterate through the text to calculate the score
        for i in self.tokenizer.tokenize(text):
            if i in self.positives:
                score += 1
            elif i in self.negatives:
                score -= 1

        return score
