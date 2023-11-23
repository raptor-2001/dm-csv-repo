import numpy as np
import cv2
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt

# Load an image
image = cv2.imread('lena.png')
image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)  # Convert BGR to RGB

# Flatten the image data to a 2D array of pixels
height, width, channels = image.shape
pixels = image.reshape((-1, channels))

# Define the number of clusters (you can adjust this based on your needs)
num_clusters = 10

# Apply K-Means clustering
kmeans = KMeans(n_clusters=num_clusters)
kmeans.fit(pixels)
cluster_centers = kmeans.cluster_centers_
cluster_labels = kmeans.labels_

# Reshape the cluster labels back to the image shape
clustered_image = cluster_labels.reshape((height, width))

# Visualize the clustered image
plt.figure(figsize=(8, 8))
plt.imshow(clustered_image, cmap='viridis')  # Choose a colormap for visualization
plt.axis('off')
plt.show()
