#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <memory>

#include <ctime>

using namespace std;

typedef double price_t;
typedef unsigned int quantity_t;

#include <uuid/uuid.h>

string generate_uuid() {
    uuid_t raw_uuid;
    uuid_generate(raw_uuid);
    char *uuid = (char *)malloc(sizeof(char) * 37);
    uuid_unparse_lower(raw_uuid, uuid);
    string result(uuid);
    return result;
}

class Address {
public:
    explicit Address(string streetNumber, string street, string zipCode, string city, string state, string country):
        streetNumber_(streetNumber), street_(street), zipCode_(zipCode), city_(city), country_(country) {}
    explicit Address() {}
    virtual ~Address() {}

    string getStreetNumber() const { return streetNumber_; }
    void setStreetNumber(const string streetNumber) { streetNumber_ = streetNumber; }
    string getStreet() const { return street_; }
    void setStreet(const string street) { street_ = street; }
    string getZipCode() const { return zipCode_; }
    void setZipCode(const string zipCode) { zipCode_ = zipCode; }
    string getCity() const { return city_; }
    void setCity(const string city) { city_ = city; }
    string getState() const { return state_; }
    void setState(const string state) { state_ = state; }
    string getCountry() const { return country_; }
    void setCountry(const string country) { country_ = country; }

private:
    string streetNumber_;
    string street_;
    string zipCode_;
    string city_;
    string state_;
    string country_;
    // Extends and add latitude and longitude
};

class CardInfo {
public:
    explicit CardInfo() {}
    explicit CardInfo(string cardNumber, string validityDate, string ccv, string holderName): cardNumber_(cardNumber), validityDate_(validityDate), ccv_(ccv), holderName_(holderName) {}
    virtual ~CardInfo() {}

    string getCardNumber() const { return cardNumber_; }
    void setCardNumber(const string cardNumber) { cardNumber_ = cardNumber; }
    string getValidityDate() const { return validityDate_; }
    void setValidityDate(const string validityDate) { validityDate_ = validityDate; }
    string getCcv() const { return ccv_; }
    void setCcv(const string ccv) { ccv_ = ccv; }
    string getHolderName() const { return holderName_; }
    void setHolderName(const string holderName) { holderName_ = holderName; }
private:
    string cardNumber_;
    string validityDate_;
    string ccv_;
    string holderName_;
};

class BillingInfo {
public:
    explicit BillingInfo() {}
    explicit BillingInfo(Address *billingAddress, CardInfo *cardInfo): billingAddress_(billingAddress), cardInfo_(cardInfo)  {}
    virtual ~BillingInfo() {}

    Address* getBillingAddress() { return billingAddress_; }
    void setBillingAddress(Address *billingAddress) { billingAddress_ = billingAddress; }
    CardInfo* getCardInfo() { return cardInfo_; }
    void setCardInfo(CardInfo *cardInfo) { cardInfo_ = cardInfo; }
private:
    Address *billingAddress_;
    CardInfo *cardInfo_;
};

class Customer {
public:
    explicit Customer() {}
    explicit Customer(string id, string firstname, string lastname, string phone, string email, Address* residencialAddress, BillingInfo *billingInfo) :
        id_(id), firstname_(firstname), lastname_(lastname), phone_(phone), email_(email), residencialAddress_(residencialAddress), billingInfo_(billingInfo) {}
    explicit Customer(string firstname, string lastname, string phone, string email, Address* residencialAddress, BillingInfo *billingInfo):
        firstname_(firstname), lastname_(lastname), phone_(phone), email_(email), residencialAddress_(residencialAddress), billingInfo_(billingInfo)
    {
        id_ = generate_uuid();
    }
    virtual ~Customer() {}

    string getId() const { return id_;}
    void setId(const string id) { id_ = id; }
    string getFirstname() const { return firstname_; }
    void setFirstname(const string firstname) { firstname_ = firstname; }
    string getLastname() const { return lastname_; }
    void setLastname(const string lastname) { lastname_ = lastname; }
    string getPhone() const { return phone_; }
    void setPhone(const string phone) { phone_ = phone; }
    string getEmail() const { return email_; }
    void setEmail(const string email) { email_ = email; }
    Address* getResidentialAddress() { return residencialAddress_; }
    void setResidencialAddress(Address* residencialAddress) { residencialAddress_ = residencialAddress; }
    BillingInfo *getBillingInfo() { return billingInfo_; }
    void setBllingInfo(BillingInfo *billingInfo) { billingInfo_ = billingInfo; }

    string to_string() {
        return firstname_ + " " + lastname_ + ", " + email_ + ", " + phone_;
    }

private:
    string id_;
    string firstname_;
    string lastname_;
    string phone_;
    string email_;
    Address *residencialAddress_;
    BillingInfo *billingInfo_;
};

class Product {
public:
    explicit Product() {
        id_ = generate_uuid();
    }
    explicit Product(string id, string name, string description, price_t price): id_(id), name_(name), description_(description), price_(price) {}

    explicit Product(string name, string description, price_t price): name_(name), description_(description), price_(price) {
        id_ = generate_uuid();
    }

    string getId() const { return id_; }
    void setId(const string id) { id_ = id; }
    string getName() const { return name_; }
    void setName(const string name) { name_ = name; }
    string getDescription() const { return description_; }
    void setDescription(const string description) { description_ = description; }
    price_t getPrice() const { return price_; }
    void setPrice(const price_t price) { price_ = price; }

    string to_string() {
        return "Product{id: " + id_ + ", name: " + name_ + ", description: " + description_ + ", price: " + std::to_string(price_) + "}";
    }

private:
    string id_;
    string name_;
    string description_;
    price_t price_;
};

class ProductLine {
public:
    explicit ProductLine() {}
    explicit ProductLine(Product *product, const quantity_t quantity): product_(product), quantity_(quantity) {}
    virtual ~ProductLine() {
        delete product_;
    }

    Product* getProduct() { return product_; }
    void setProduct(Product* product) { product_ = product; }
    quantity_t getQuantity() const { return quantity_; }
    void setQuantity(const quantity_t quantity) { quantity_ = quantity; }

    string to_string() {
        return "ProductLine{product: " + product_->to_string() + ", quantity: " + std::to_string(quantity_) + ", price: " + std::to_string(getPrice()) + "}";
    }

    price_t getPrice() {
        return product_->getPrice() * quantity_;
    }

private:
    Product *product_;
    quantity_t quantity_;
};

class Order {
public:
    explicit Order() {}
    virtual ~Order() {
        vector<ProductLine*>::iterator it;
        for(it = lines_.begin(); it < lines_.end(); it++) {
            delete (*it);
        }
    }

    void addLine(Product *product, quantity_t quantity) {
        addLine(new ProductLine(product, quantity));
    }

    void addLine(ProductLine *line) {
        lines_.push_back(line);
    }

    vector<ProductLine*> getLines() const { return lines_; }

    Customer* getCustomer() { return customer_; }
    void setCustomer(Customer* customer) { customer_ = customer; }

    string to_string() {
        string s = "Order information: \n";
        s = s + "Customer: " + customer_->to_string() + "\n";
        vector<ProductLine*>::iterator it;
        for(it = lines_.begin(); it < lines_.end(); it++) {
            s = s + (*it)->to_string() + "\n";
        }
        return s;
    }

    bool isValidate() { return validate_; }
    void validate() { validate_ = true; }
    void invalidate() { validate_ = false; }

    void place() {}

private:
    vector<ProductLine*> lines_;
    Customer *customer_;
    bool validate_ = false;
};

int main()
{
    Address *address = new Address("4", "angel street", "514H3W", "Montreal", "Quebec", "Canada");
    CardInfo *cardInfo = new CardInfo("4444333322221111", "06/20", "000", "John Doe");
    BillingInfo *billingInfo = new BillingInfo(address, cardInfo);
    Customer *cust = new Customer("john", "doe", "+12546895", "jdoe@mydom.com", address, billingInfo);

    Order *order = new Order();
    order->addLine(new ProductLine(new Product("product1", "First product", 13.99), 4));
    order->setCustomer(cust);

    cout << "------------------ Simple test -------------------------" << endl;

    cout << order->to_string() << endl;

    cout << "--------------------------------------------------------" << endl;

    delete order;
    delete address;
    delete cardInfo;
    delete billingInfo;
    delete cust;

    return 0;
}
