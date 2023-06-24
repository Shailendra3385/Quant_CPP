#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// Define the order data structure
struct Order {
    int id;
    char type; // 'B' for buy, 'S' for sell
    double price;
    int quantity;
    int timestamp;
};

// Define the order book data structure
class OrderBook {
public:
    void placeOrder(Order order);
    void cancelOrder(int orderId);
    void matchOrders();

    void display();

private:
    map<double, vector<Order>> buyOrders;
    map<double, vector<Order>> sellOrders;
};

// Implement the order placement function
void OrderBook::placeOrder(Order order) {
    if (order.type == 'B') {
        buyOrders[order.price].push_back(order);
        sort(buyOrders[order.price].begin(), buyOrders[order.price].end(),
            [](Order a, Order b) { return a.timestamp < b.timestamp; });
    }
    else {
        sellOrders[order.price].push_back(order);
        sort(sellOrders[order.price].begin(), sellOrders[order.price].end(),
            [](Order a, Order b) { return a.timestamp < b.timestamp; });
    }
}

// Implement the order cancellation function
void OrderBook::cancelOrder(int orderId) {
    for (auto& buyOrder : buyOrders) {
        auto& orders = buyOrder.second;
        for (auto it = orders.begin(); it != orders.end(); it++) {
            if (it->id == orderId) {
                orders.erase(it);
                return;
            }
        }
    }

    for (auto& sellOrder : sellOrders) {
        auto& orders = sellOrder.second;
        for (auto it = orders.begin(); it != orders.end(); it++) {
            if (it->id == orderId) {
                orders.erase(it);
                return;
            }
        }
    }
}

// Implement the order matching function
void OrderBook::matchOrders() {
    for (auto& buyOrder : buyOrders) {
        double buyPrice = buyOrder.first;
        auto& buyOrdersList = buyOrder.second;

        if (sellOrders.empty()) {
            return;
        }

        auto sellOrder = sellOrders.begin()->second.front();
        double sellPrice = sellOrder.price;
        auto& sellOrdersList = sellOrders[sellPrice];

        if (buyPrice < sellPrice) {
            return;
        }

        int quantity = min(buyOrdersList.front().quantity, sellOrdersList.front().quantity);

        buyOrdersList.front().quantity -= quantity;
        sellOrdersList.front().quantity -= quantity;
		
		cout << "Trade: Buy order " << buyOrdersList.front().id << " and sell order "
             << sellOrdersList.front().id << " for " << quantity << " shares at price "
             << sellPrice << endl;

        if (buyOrdersList.front().quantity == 0) {
            buyOrdersList.erase(buyOrdersList.begin());
        }
        if (sellOrdersList.front().quantity == 0) {
            sellOrdersList.erase(sellOrdersList.begin());
            if (sellOrdersList.empty()) {
                sellOrders.erase(sellPrice);
            }
        }
    }
}

// Implement the order book display function
void OrderBook::display() {
    cout << "Buy Orders:" << endl;
    for (auto it = buyOrders.rbegin(); it != buyOrders.rend(); it++) {
        double price = it->first;
        auto orders = it->second;
        cout << "Price: " << price << endl;
		for(auto& o:orders)
		{
			cout << "id: " << o.id << " ,type: " << o.type << " ,quantity: " << o.quantity << endl;
		}
	}
	cout << "Sell Orders:" << endl;
    for (auto it = sellOrders.rbegin(); it != sellOrders.rend(); it++) {
        double price = it->first;
        auto orders = it->second;
        cout << "Price: " << price << endl;
		for(auto& o:orders)
		{
			cout << "id: " << o.id << " ,type: " << o.type << " ,quantity: " << o.quantity << endl;
		}
	}
}
int main()
{
	OrderBook Ob;
	Order o1={1,'B',100.0,10,1};
	Ob.placeOrder(o1);
	Order o2={2,'S',100.5,10,2};
	Ob.placeOrder(o2);
	Order o3={3,'S',100,5,3};
	Ob.placeOrder(o3);
	Ob.display();
	Ob.matchOrders();
	return 0;
}

