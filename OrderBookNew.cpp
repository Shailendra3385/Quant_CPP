#include <iostream>
#include <queue>
#include <vector>
#include <functional>

enum class OrderType {
    BUY, SELL
};

struct Order {
    OrderType type;
    double price;
    int quantity;
    int timestamp;

    Order(OrderType type, double price, int quantity, int timestamp)
        : type(type), price(price), quantity(quantity), timestamp(timestamp) {}
};

bool operator<(const Order& lhs, const Order& rhs) {
    if (lhs.price == rhs.price) {
        return lhs.timestamp > rhs.timestamp;
    }
    return lhs.price < rhs.price;
}

bool operator>(const Order& lhs, const Order& rhs) {
    if (lhs.price == rhs.price) {
        return lhs.timestamp < rhs.timestamp;
    }
    return lhs.price > rhs.price;
}

class OrderBook {
private:
    std::priority_queue<Order, std::vector<Order>, std::less<>> buyOrders;
    std::priority_queue<Order, std::vector<Order>, std::greater<>> sellOrders;

public:
    void addOrder(const Order& order) {
        if (order.type == OrderType::BUY) {
            buyOrders.push(order);
        } else if (order.type == OrderType::SELL) {
            sellOrders.push(order);
        }
    }

    void matchOrders() {
        while (!buyOrders.empty() && !sellOrders.empty() && buyOrders.top().price >= sellOrders.top().price) {
            auto buyOrder = buyOrders.top();
            auto sellOrder = sellOrders.top();
            int matchedQuantity = std::min(buyOrder.quantity, sellOrder.quantity);
            std::cout << "Matched " << matchedQuantity << " at price " << sellOrder.price << std::endl;
            buyOrder.quantity -= matchedQuantity;
            sellOrder.quantity -= matchedQuantity;
            buyOrders.pop();
            sellOrders.pop();
            if (buyOrder.quantity > 0) {
                buyOrders.push(buyOrder);
            }
            if (sellOrder.quantity > 0) {
                sellOrders.push(sellOrder);
            }
        }
    }
    
    void displayAndClearBuyOrders() {
        std::cout << "Buy Orders: \n";
        while (!buyOrders.empty()) {
            auto order = buyOrders.top();
            std::cout << "Price: " << order.price << ", Quantity: " << order.quantity << ", Time: " << order.timestamp << std::endl;
            buyOrders.pop();
        }
    }

    void displayAndClearSellOrders() {
        std::cout << "Sell Orders: \n";
        while (!sellOrders.empty()) {
            auto order = sellOrders.top();
            std::cout << "Price: " << order.price << ", Quantity: " << order.quantity << ", Time: " << order.timestamp << std::endl;
            sellOrders.pop();
        }
    }
};

int main() {
    OrderBook orderBook;

    orderBook.addOrder(Order(OrderType::SELL, 100.0, 10, 1));
    orderBook.addOrder(Order(OrderType::BUY, 105.0, 15, 2));
    orderBook.addOrder(Order(OrderType::BUY, 101.0, 5, 3));
    orderBook.addOrder(Order(OrderType::SELL, 99.0, 8, 4));
    orderBook.addOrder(Order(OrderType::SELL, 103.0, 10, 5));

    orderBook.matchOrders();

    // Display and clear orders: Note, doing so will remove all orders from the order book
    orderBook.displayAndClearBuyOrders();
    orderBook.displayAndClearSellOrders();

    return 0;
}