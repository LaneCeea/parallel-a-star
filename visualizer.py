import lib.Core as Core
import matplotlib.pyplot as plt

print(f'cpp API call: add(1, 2) = {Core.add(1, 2)}')

print('Testing matplotlib')

# Simple data for the plot
x = [1, 2, 3, 4, 5]
y = [1, 4, 9, 16, 25]

# Create a basic plot
plt.plot(x, y, label='y = x^2')

# Add a title, labels, and a legend
plt.title("Hello, Matplotlib!")
plt.xlabel("X-axis")
plt.ylabel("Y-axis")
plt.legend()

# Display the plot
plt.show()
