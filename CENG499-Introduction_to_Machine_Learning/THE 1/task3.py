import torch
import torch.nn as nn
import torch.nn.functional as F
import torchvision

class MyModel(nn.Module):
    num_channels = 1
    num_outputs = 10

    def __init__(self):
        super().__init__()
        # write your code here
        self.c1 = nn.Conv2d(self.num_channels, 5, kernel_size=5)
        self.c2 = nn.Conv2d(5, 20, kernel_size=5)
        self.c2_drop = nn.Dropout2d()
        self.fc1 = nn.Linear(320, 60)
        self.fc2 = nn.Linear(60, self.num_outputs)

    def forward(self, x):
        # write your code here
        x = F.gelu(F.max_pool2d(self.c1(x), 2))
        x = F.gelu(F.max_pool2d(self.c2_drop(self.c2(x)), 2))
        x = x.view(-1, 320)
        x = F.gelu(self.fc1(x))
        x = F.dropout(x, p=0.5, training=self.training)
        x = self.fc2(x)
        return F.log_softmax(x, dim=1)


def main():

    def train():
        model.train()
        total_loss = 0
        for batch_idx, (data, target) in enumerate(train_loader):
            optimizer.zero_grad()
            output = model(data)
            loss = F.nll_loss(output, target)
            loss.backward()
            optimizer.step()
            total_loss += loss.item()
        print("\nepoch:", epoch+1, " loss:", total_loss / train_loader.__len__())

    def test():
        model.eval()
        correct = 0
        with torch.no_grad():
            for data, target in test_loader:
                output = model(data)
                _, pred = torch.max(output.data, 1)
                correct += (pred == target).sum().item()
        print("\nAccuracy on test set: % {}".format(100. * correct / len(test_loader.dataset)))

    torch.backends.cudnn.enabled = False
    torch.manual_seed(1234)

    train_loader = torch.utils.data.DataLoader(
        torchvision.datasets.MNIST('/files/', train=True, download=True,
                                   transform=torchvision.transforms.Compose([
                                       torchvision.transforms.ToTensor(),
                                       torchvision.transforms.Normalize(
                                           (0.1250,), (0.3100,))
                                   ])),
        batch_size=64, shuffle=True)

    test_loader = torch.utils.data.DataLoader(
        torchvision.datasets.MNIST('/files/', train=False, download=True,
                                   transform=torchvision.transforms.Compose([
                                       torchvision.transforms.ToTensor(),
                                       torchvision.transforms.Normalize(
                                           (0.1305,), (0.3070,))
                                   ])),
        batch_size=1000, shuffle=True)

    model = MyModel()
    optimizer = torch.optim.SGD(model.parameters(), lr=0.01,
                          momentum=0.9)


    for epoch in range(3):
        train()

    test()

if __name__ == '__main__':
    main()