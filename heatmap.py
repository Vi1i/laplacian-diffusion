import plotly.plotly as py
import plotly.graph_objs as go
import pandas as pd

df = pd.read_csv('1Dout.csv')


data = [go.Heatmap(z=df.values.tolist(), colorscale='Viridis')]
py.iplot(data, filename='1D')
