#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

template <typename T, int Cap>
class Registry {
private:
  std::vector<T> data_;
public:
  void add(const T& item) {
    if (data_.size() < Cap) {
      data_.push_back(item);
    }
  }
  size_t size() const { return data_.size(); }
  const T& operator[](size_t index) const { return data_[index]; }
  void print() const {
    for (const auto& item : data_) {
      std::cout << item << std::endl;
    }
  }
};

// Ошибки
//-------------------------------
class FleetError : public std::exception {
protected:
  std::string msg_;
public:
  FleetError(std::string msg) : msg_(std::move(msg)) {}
  const char* what() const noexcept override { return msg_.c_str(); }
};

class InspectionError : public FleetError {
public:
  explicit InspectionError(const std::string& msg) : FleetError(msg) {}
};

class ParseError : public FleetError {
public:
  explicit ParseError(const std::string& msg) : FleetError(msg) {}
};
//-------------------------------


// Транспорт
//-------------------------------
class InventoryItem {
public:
  virtual ~InventoryItem() = default;
  virtual std::string id() const = 0;
  virtual std::string name() const = 0;
  virtual std::string describe() const = 0;
  bool operator<(const InventoryItem& other) const {
    return id() < other.id();
  }
};

std::ostream& operator<<(std::ostream& os, const InventoryItem& item) {
  os << item.name() << " " << item.id();
  return os;
}

class EnergyConsumer {
public:
  virtual ~EnergyConsumer() = default;
  virtual double dailyEnergy() const = 0;
};

class Transport : public InventoryItem {
protected:
  std::string id_;
  std::string name_;
  int simplicity_;
public:
  Transport(std::string id, std::string name, int simplicity) : id_(std::move(id)), name_(std::move(name)), simplicity_(simplicity) {
    if (simplicity < 1 || simplicity > 10) {
      throw ParseError("Simplicity out of bounds (must be 1-10)!");
    }
  }
  std::string id() const override { return id_; }
  std::string name() const override { return name_; }
  int simplicity() const { return simplicity_; }
};

class Bike : public Transport {
public:
  Bike(std::string id, std::string name, int simplicity) : Transport(std::move(id), std::move(name), simplicity) {}
  std::string describe() const override { 
    return "bike " + name_ + " " + id_;
  }
};

class Helmet : public InventoryItem {
protected:
  std::string id_;
  std::string name_;
public:
  Helmet(std::string id, std::string name) : id_(std::move(id)), name_(std::move(name)) {}
  std::string id() const override { return id_; }
  std::string name() const override { return name_; }
  std::string describe() const override {
    return "helmet " + name_ + " " + id_;
  }
};

class Dock : public InventoryItem, public EnergyConsumer {
protected:
  std::string id_;
  std::string name_;
  double dailyEnergy_;
public:
  Dock(std::string id, std::string name, double dailyEnergy) : id_(std::move(id)), name_(std::move(name)), dailyEnergy_(dailyEnergy) {}
  std::string id() const override { return id_; }
  std::string name() const override { return name_; }
  std::string describe() const override {
    return "dock " + name_ + " " + id_;
  }
  double dailyEnergy() const override { return dailyEnergy_; }
};

class ElectricScooter : public Transport, public EnergyConsumer {
protected:
  double dailyEnergy_;
public:
  ElectricScooter(std::string id, std::string name, int simplicity, double dailyEnergy) : Transport(std::move(id), std::move(name), simplicity), dailyEnergy_(dailyEnergy) {}
  std::string describe() const override {
    return "scooter " + name_ + " " + id_;
  }
  double dailyEnergy() const override { return dailyEnergy_; }
};

class EBike : public Transport, public EnergyConsumer {
protected:
  double dailyEnergy_;
public:
  EBike(std::string id, std::string name, int simplicity, double dailyEnergy) : Transport(std::move(id), std::move(name), simplicity), dailyEnergy_(dailyEnergy) {}
  std::string describe() const override {
    return "ebike " + name_ + " " + id_;
  }
  double dailyEnergy() const override { return dailyEnergy_; }
};

class ServiceCenter {
public:
  void inspect(const InventoryItem& item) {
    std::string name = item.name();
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    if (lowerName.find("broken") != std::string::npos) {
      throw InspectionError("Inspection rejected " + name);
    }
  }
};

class Park {
private:
  std::vector<std::shared_ptr<InventoryItem>> items_;
  std::weak_ptr<InventoryItem> focus_;
public:
  void add(std::unique_ptr<InventoryItem> item) {
    items_.push_back(std::move(item));
  }
  void focus(const std::string& id) {
    for (const auto& item : items_) {
      if (item->id() == id) {
        focus_ = item;
        return;
      }
    }
  }
  void show_focus() const {
    auto shared = focus_.lock();
    if (shared) {
      std::cout << shared->describe() << std::endl;
    }
    else {
      std::cout << "expired" << std::endl;
    }
  }
  void remove(const std::string& id) {
    auto trash_start = std::remove_if(items_.begin(), items_.end(), [&id](const std::shared_ptr<InventoryItem>& item) {
      return item->id() == id;
      });
    items_.erase(trash_start, items_.end());
  }
  void list() const {
    auto sortedItems = items_;
    std::sort(sortedItems.begin(), sortedItems.end(), [](const std::shared_ptr <InventoryItem>& a, const std::shared_ptr<InventoryItem>& b) {
      return a->id() < b->id();
      });
    for (const auto& item : sortedItems) {
      std::cout << *item << std::endl;
    }
  }
  void energy() const {
    double total = 0.0;
    for (const auto& item : items_) {
      if (auto consumer = dynamic_cast<EnergyConsumer*>(item.get())) {
        total += consumer->dailyEnergy();
      }
    }
    std::cout << std::fixed << std::setprecision(1) << total << std::endl;
  }
  void novice() const {
    for (const auto& item : items_) {
      if (auto transport = dynamic_cast<Transport*>(item.get())) {
        if (transport->simplicity() >= 6) {
          std::cout << *item << std::endl;
        }
      }
    }
  }
};
//-------------------------------

int main() {
  Park park;
  ServiceCenter service_center;
  Registry<std::string, 10> history;
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) continue;
    std::istringstream iss(line);
    std::string command;
    iss >> command;
    try {
      if (command == "add") {
        std::string type, id, name;
        iss >> type >> id >> name;
        if (type == "scooter") {
          double dailyEnergy;
          int simplicity;
          iss >> dailyEnergy >> simplicity;
          auto item = std::make_unique<ElectricScooter>(id, name, simplicity, dailyEnergy);
          service_center.inspect(*item);
          park.add(std::move(item));
          history.add("Added scooter " + id);
        }
        else if (type == "bike") {
          int score;
          iss >> score;
          auto item = std::make_unique<Bike>(id, name, score);
          service_center.inspect(*item);
          park.add(std::move(item));
          history.add("Added bike " + id);
        }
        else if (type == "ebike") {
          double energy; int score;
          iss >> energy >> score;
          auto item = std::make_unique<EBike>(id, name, score, energy);
          service_center.inspect(*item);
          park.add(std::move(item));
          history.add("Added ebike " + id);
        }
        else if (type == "helmet") {
          auto item = std::make_unique<Helmet>(id, name);
          service_center.inspect(*item);
          park.add(std::move(item));
          history.add("Added helmet " + id);
        }
        else if (type == "dock") {
          double energy;
          iss >> energy;
          auto item = std::make_unique<Dock>(id, name, energy);
          service_center.inspect(*item);
          park.add(std::move(item));
          history.add("Added dock " + id);
        }
        else {
          throw ParseError("Unknown item type");
        }
      }
      else if (command == "energy") {
        park.energy();
      }
      else if (command == "novice") {
        park.novice();
      }
      else if (command == "list") {
        park.list();
      }
      else if (command == "focus") {
        std::string id; iss >> id;
        park.focus(id);
      }
      else if (command == "show_focus") {
        park.show_focus();
      }
      else if (command == "remove") {
        std::string id; iss >> id;
        park.remove(id);
      }
      else if (command == "quit") {
        std::cout << "--- History ---" << std::endl;
        history.print();
        break;
      }
      else {
        throw ParseError("Unknown command");
      }
    }
    catch (const std::exception& e) {
      std::cout << "error: " << e.what() << std::endl;
      history.add(std::string(e.what()));
    }
  }
  return 0;
}