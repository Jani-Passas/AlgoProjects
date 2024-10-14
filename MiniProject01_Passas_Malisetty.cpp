#include <iostream>
using namespace std;

class Node{
    public:
    int data;
    Node *next;

    Node(){
        data = 0;
        next = NULL;
    }

    Node(int data){
        this->data = data;
        this->next = NULL;
    }
};


class BookingSingle{
    Node *head;
    public:
    BookingSingle(){
        head = NULL;
    }

    Node* getHead(){
        return head;
    }

    void insertBooking(int value){
        Node* newnode = new Node(value);
        if(head == nullptr){
            head = newnode;
        }else{
            Node* temp = head;
            while(temp->next != nullptr){
                temp = temp->next;
            }
            temp->next = newnode;
        }

    }

    void cancelBooking(int value){
        Node* temp = head;
        Node* del = nullptr;

        if(temp != nullptr && temp->data == value){ //case where head is the booking to cancel
            head = head->next;
            cout << "Seat booking: " << value << " has been deleted" << endl;
            delete temp;
            return;
        }

        //iterate until reaching end of list, or the next data is equal to the value
        while(temp != nullptr && temp->next != nullptr && temp->next->data != value){
            temp = temp ->next;
        }

        if(temp->next == nullptr){//if end of list is reached
            cout << "Cannot delete this reservation, " << value << " not found." << endl;
            return;
        }else{//otherwise
            del = temp->next;
            temp->next = temp->next->next;
            cout << "Seat booking: " << value << " has been deleted" << endl;
            delete del;
            return;
        }
    }

    void displayBookings(){
        Node* temp = head;
        if(head == NULL){
            cout << "No items in the booking system!" << endl;
            return;
        }
        while(temp!=NULL){
            cout << temp->data << " -> ";
            temp = temp->next;
        }

        cout << "NULL" << endl;
    }
};


int main(){
    BookingSingle booking;
    booking.insertBooking(1);
    booking.displayBookings();
    booking.insertBooking(2);
    booking.insertBooking(3);
    booking.displayBookings();
    booking.cancelBooking(2);
    booking.displayBookings();

    return 0;
}

