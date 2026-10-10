CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC = src/models/MenuItem.cpp \
      src/models/Restaurant.cpp \
      src/models/User.cpp \
      src/models/DeliveryPartner.cpp \
      src/models/OrderItem.cpp \
      src/models/Payment.cpp \
      src/models/Coupon.cpp \
      src/models/Review.cpp \
      src/models/Order.cpp \
      src/strategies/IPartnerAssignmentStrategy.cpp \
      src/strategies/IPaymentStrategy.cpp \
      src/observers/OrderObservers.cpp \
      src/repositories/InMemoryRestaurantRepository.cpp \
      src/repositories/InMemoryDeliveryPartnerRepository.cpp \
      src/repositories/InMemoryOrderRepository.cpp \
      src/repositories/InMemoryCouponRepository.cpp \
      src/services/PricingService.cpp \
      src/services/FoodDeliveryService.cpp \
      src/main.cpp

TARGET = food_sprint

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe
