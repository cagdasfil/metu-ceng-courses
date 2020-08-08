from task2 import linear_regression

train_path, test_path = "task2\\train_set.txt", "task2\\test_set.txt"
rmse = linear_regression(train_path, test_path, num_epochs=1000, learning_rate=0.001)
print("RMSE = {} for {}".format(rmse, test_path))
