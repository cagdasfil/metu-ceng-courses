import torch
import torchvision.transforms as transforms
from torchvision.datasets import CIFAR10
import torch.nn as nn
import torch.optim as optim

class Model:
    def __init__(self):
        torch.manual_seed(1234)
        ratio = 0.1
        transform = transforms.Compose(
            [transforms.ToTensor(),
             transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])

        train_set = CIFAR10(root="task2_data", train=True,
                            transform=transform, download=True)
        test_set = CIFAR10(root="task2_data", train=False,
                           transform=transform, download=True)
        train_set, val_set = torch.utils.data.random_split(train_set,
                                                           [int((1 - ratio) * len(train_set)),
                                                            int(ratio * len(train_set))])

        self.train_loader = torch.utils.data.DataLoader(train_set, batch_size=500,
                                                  shuffle=True, num_workers=2)
        self.val_loader = torch.utils.data.DataLoader(val_set, batch_size=500,
                                                      shuffle=False, num_workers=2)
        self.test_loader = torch.utils.data.DataLoader(test_set, batch_size=500,
                                                 shuffle=False, num_workers=2)


        self.input_nodes = 3072
        self.output_nodes = 10

        self.layers = None
        self.act_func = None
        self.l_rate = None
        self.model = None
        self.optim = None
        self.loss_func = None

    def init_model(self, layers, func, rate):
        self.layers = layers
        functions = {1: nn.LeakyReLU, 2: nn.SELU, 3: nn.GELU}
        self.act_func = functions[func]
        self.l_rate = rate

        if self.layers == 1:
            self.model = nn.Sequential(nn.Linear(self.input_nodes, self.output_nodes))
        elif self.layers == 2:
            self.model = nn.Sequential(nn.Linear(self.input_nodes, 500),
                                       self.act_func(),
                                       nn.Linear(500, self.output_nodes))
        elif self.layers == 3:
            self.model = nn.Sequential(nn.Linear(self.input_nodes, 500),
                                       self.act_func(),
                                       nn.Linear(500, 500),
                                       self.act_func(),
                                       nn.Linear(500, self.output_nodes))

        self.loss_func = nn.CrossEntropyLoss()
        self.optim = optim.Adam(self.model.parameters(), lr=self.l_rate)

    def train(self):
        for epoch in range(20):

            total_loss = 0.0
            for inputs, labels in self.train_loader:

                inputs = inputs.view(inputs.shape[0], -1)

                self.optim.zero_grad()

                outputs = self.model(inputs)
                loss = self.loss_func(outputs, labels)
                loss.backward()
                self.optim.step()

                total_loss += loss.item()

            print("\nepoch:", epoch + 1)
            print("training loss:", total_loss / self.train_loader.__len__())

            self.validation()

    def validation(self):
        correct = 0
        total = 0
        with torch.no_grad():
            total_loss = 0.0
            for inputs, labels in self.val_loader:
                inputs = inputs.view(inputs.shape[0], -1)
                outputs = self.model(inputs)
                loss = self.loss_func(outputs, labels)
                total_loss += loss.item()
                _, predicted = torch.max(outputs.data, 1)
                total += labels.size(0)
                correct += (predicted == labels).sum().item()
        print("validation loss:", total_loss / self.val_loader.__len__())
        print('Accuracy on validation set:', (100 * correct / total))

    def test(self):
        correct = 0
        total = 0
        with torch.no_grad():
            for inputs, labels in self.test_loader:
                inputs = inputs.view(inputs.shape[0], -1)
                outputs = self.model(inputs)
                _, predicted = torch.max(outputs.data, 1)
                total += labels.size(0)
                correct += (predicted == labels).sum().item()
        print('\n****** RESULT ******\nAccuracy on test set:', (100 * correct / total))
        print("********************")

def main():
    number_of_layers = 0
    activation_func = 0
    LR = 0
    wrong = True

    while wrong:
        number_of_layers = input("Please enter hidden layer numbers: ")
        if number_of_layers in ["1", "2", "3"]:
            wrong = False
        else:
            print("ERROR: valid numbers are 1,2 and 3")
    wrong = True
    while wrong:
        activation_func = input("[1:LeakyReLU, 2:SELU, 3:GELU]\nPlease choose activation function number: ")
        if activation_func in ["1", "2", "3"]:
            wrong = False
        else:
            print("ERROR: valid numbers are 1,2 and 3")
    wrong = True
    while wrong:
        LR = input("Please enter learning rate: ")
        if LR in ["0.1", "0.03", "0.01", "0.003", "0.001"]:
            wrong = False
        else:
            print("ERROR: valid numbers are 0.1, 0.03, 0.01, 0.003 and 0.001")

    model = Model()
    model.init_model(int(number_of_layers), int(activation_func), float(LR))
    model.train()
    model.test()


if __name__ == "__main__":
    main()