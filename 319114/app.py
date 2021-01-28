import base64
from io import BytesIO

import numpy as np
import matplotlib

matplotlib.use("Agg")
from matplotlib import pyplot as plt
from flask import Flask, render_template

app = Flask(__name__)


def fig_to_base64_img(fig):
    """画像を base64 に変換する。
    """
    # png 形式で出力する。
    io = BytesIO()
    fig.savefig(io, format="png")
    # base64 形式に変換する。
    io.seek(0)
    base64_img = base64.b64encode(io.read()).decode()

    return base64_img


def create_graph():
    """matplotlib のグラフを作成する。
    """
    x = np.linspace(-10, 10, 100)
    y = x ** 2

    fig, ax = plt.subplots()
    ax.plot(x, y)
    ax.set_title("Title", c="darkred", size="large")

    return fig


@app.route("/")
def hello():
    # グラフを作成する。
    fig = create_graph()
    # グラフを画像化して、base64 形式にエンコードする。
    img = fig_to_base64_img(fig)

    return render_template("index.html", img=img)


if __name__ == "__main__":
    app.run(debug=True)
