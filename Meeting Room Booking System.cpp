//Meeting room booking system
//C++ standard library
#include <iostream> 
#include <string>
#include <fstream> 
#include <iomanip> 
#include <ctime> 
#include <cmath>
using namespace std;

const string DAY[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
const string TIMESLOT[10]={"09:00am-10:00am","10:00am-11.00am","11.00am-12.00pm","12.00pm-1.00pm","1.00pm-2.00pm","2.00pm-3.00pm","3.00pm-4.00pm","4.00pm-5.00pm","5.00pm-6.00pm","6.00pm-7.00pm"};

bool withinSixDays(int,int,int);

class Date{
	private:
		int day;  //day of the booking date
		int month; //month of the booking date
		int year; //year of the booking date
		int dd; //day of the week (0-Sunday, 6-Saturday)
		int timeslot; //time slot(1 - 10)
	public:
		//default constructor with default arguments
		Date(int day=0,int month=0,int year=0,int dd=0,int timeslot=0)
		{
			this->day=day;
			this->month=month;
			this->year=year;
			this->dd=dd;
			this->timeslot=timeslot;
		}
		//set day
		void setDay(int d)
		{
			day=d;
		}
		//set month
		void setMonth(int m)
		{
			month=m;
		}
		//set year
		void setYear(int y)
		{
			year=y;
		}
		//set day of the week (0-Sunday, 6- Saturday)
		void setDD(int d)
		{
			dd=d;
		}
		//set timeslot
		void setTimeslot(int t)
		{
			timeslot=t;
		}
		//return day
		int getDay() const{return day;}
		//return month
		int getMonth() const{return month;}
		//return year
		int getYear() const{return year;}
		//return day of the week (0-Sunday, 6-Saturday)
		int getDD() const{return dd;}
		//return time slot
		int getTimeSlot() const{return timeslot;}
		
		//return date
		string getDate()const
		{
			string tempD, tempMM, tempYYYY;		
			if(day<10)					    //if day is less than 10
				tempD="0"+to_string(day);	//then add "0" with the day in string
			else						    //if day greater or equal to 10
				tempD=to_string(day);		//change the integer value of month to string value
			if(month<10)				     //if month is less than 10
				tempMM="0"+to_string(month);  //then add "0" with the month in string
			else						//if month greater or equal to 10
				tempMM=to_string(month);	//change the integer value of month to string value
			tempYYYY=to_string(year);	//change the integer value of day to string value
			return tempD+"-"+tempMM+"-"+tempYYYY;	//return the standard expression of date 
		}
		//read the booking date from users
		void readBookingDate()
		{
			char hyphen;
			time_t today = time(0);
			struct tm *ltm = gmtime(&today);
			cout<<"Today's date: "<<ltm->tm_mday<<"-"<<ltm->tm_mon+1<<"-"<<ltm->tm_year+1900<<endl;
			cout<<"Enter the date in (dd-mm-yyyy) format:";
			cin>>day>>hyphen>>month>>hyphen>>year;
			
			while(!withinSixDays(day,month,year))
			{
				cout<<"You can only book a meeting room within 6 days starting from tomorrow. "<<endl;
				cout<<"Enter the date in (dd-mm-yyyy) format:";
				cin>>day>>hyphen>>month>>hyphen>>year;
			}
			
			struct tm bookingDate = {0,0,0,day,month-1,year-1900};
			mktime(&bookingDate);
			dd = bookingDate.tm_wday;
			
			cout<<"\nYou can select the timeslot below."<<endl
					<<"1) 09:00am-10:00am"<<endl
					<<"2) 10:00am-11.00am"<<endl
					<<"3) 11.00am-12.00pm"<<endl
					<<"4) 12.00pm-1.00pm"<<endl
					<<"5) 1.00pm-2.00pm"<<endl
					<<"6) 2.00pm-3.00pm"<<endl
					<<"7) 3.00pm-4.00pm"<<endl
					<<"8) 4.00pm-5.00pm"<<endl
					<<"9) 5.00pm-6.00pm"<<endl
					<<"10)6.00pm-7.00pm"<<endl<<endl;
			do
			{		
				cout<<"Select the timeslot (1-10) : ";
				cin>>timeslot;
			}while(timeslot<1 || timeslot >10);
			
		}	
};

class MeetingRoomNode{
	public:
		MeetingRoomNode* next;  //pointer pointing to the next node
		int roomNo;             //room number of the type of meeting room
		string style;           //style of meeting room
		string size;            //size of meeting room
		double price;           //price per person for booking a meeting room
		string occupied[7][10]; //availability of meeting room according to day and time slot
		void initialOccupied()  //initialise the occupied array to be '_' (empty)
		{
			for(int i=0;i<7;i++)
				for(int j=0;j<10;j++)
					occupied[i][j]="_";
		}
		//default constructor with default arguments
		MeetingRoomNode(int roomNo=0,string style="",string size="",double price=0)
		{
			next = NULL;
			this->roomNo = roomNo;
			this->style  = style;
			this->size	 = size;
			this->price=price;
			initialOccupied();
		}	
		//set the room number
		void setRoomNo(int roomNo) 
		{
			this->roomNo = roomNo;
		}
		//set the style of meeting room
		void setStyle(string style)
		{
			this->style = style;
		}
		//set the size of meeting room
		void setSize(string size)
		{
			this->size = size;
		}
		//set the price per person of the type of meeting room
		void setPrice(double p)
		{
			price = p;
		}
		//set the meeting room occupied at a specific time slot
		void setOccupied(int i,int j)
		{
			occupied[i][j] = "X";
		}
		//set the meeting room unoccupied at a specific time slot
		void setUnoccupied(int i,int j)
		{
			occupied[i][j] = "_";
		}
		//return the room number
		int getRoomNo() const{return roomNo;}
		//return the style of meeting room
		string getStyle() const{return style;}
		//return the size of meeting room
		string getSize() const{return size;}
		//return the price per person for booking a meeting room
		double getPrice()const{return price;}
		//return the availability of a meeting room at a specific time 
		string getOccupied(int i,int j) const{return occupied[i][j];}
};

class MeetingRoomQueue{
	private:
		MeetingRoomNode* front; ///pointer pointing to the first node in MeetingRoom queue
		MeetingRoomNode* rear;  //pointer pointing to the last node in MeetingRoom queue
		
	public:
		//constructor
		MeetingRoomQueue()
		{
			front = rear = NULL;		//MeetingRoom queue is empty initially,front and rear is set to NULL
		}
		//destructor
		~MeetingRoomQueue()
		{
			MeetingRoomNode *temp = front;
			while(temp)
			{
				front = temp->next;
				delete temp;		//destroy the current meeting room node
				temp = front;
			}
		}
		//return true if the MeetingRoomQueue is empty
		bool isEmpty_room()
		{
			return front == NULL && rear == NULL;
		}
		//insert meeting room node to the rear of MeetingRoom queue
		void enQueue_room(int roomNo=0,string style="",string size="",double price=0)
		{
			//Create a new meeting room node
			MeetingRoomNode* newNode=new MeetingRoomNode(roomNo, style, size, price);
			
			//Insert to an empty MeetingRoom queue
			if(isEmpty_room())
			{
				front = rear = newNode;
			}
			//Insert to a non-empty MeetingRoom queue
			else
			{
				newNode->next = NULL;
				rear->next = newNode;
				rear = newNode;
			}
		}
		//delete a meeting room node from the front of MeetingRoom queue
		void deQueue_room()
		{
			//Cannot delete the meeting room node from queue if the MeetingRoom queue is empty
			if(isEmpty_room())
			{
				cout<<"The meeting room queue is currently empty. No meeting room to delete.\n";
				return;
			}
			
			//Delete from the front of the MeetingRoom queue
			MeetingRoomNode *temp = front;
			front = front->next;
			delete temp;
			
			//If the MeetingRoom queue contains only one MeetingRoom node
			if(front == NULL)
				rear = NULL;
		}
		//if the room number of a meeting room node is found the same with roomNo passed as argument, return its position  otherwise return 0
		int findNode(int roomNo)
		{
			MeetingRoomNode * currNode = front;
			int currIndex=1;
			while(currNode && currNode->roomNo!=roomNo)
			{
				currNode=currNode->next;
				currIndex++;
			}
			//return the position of the meeting room node with the matched roomNo
			if(currNode)
			{
				return currIndex;
			}
			//return 0 if the meeting room node with the searched roomNo is not found
			else
			{
				return 0;
			}	
		}	
		//return a pointer pointing to the room node with roomNo passed as argument 
		MeetingRoomNode* assignRoom(int roomNo)
		{
			MeetingRoomNode * currNode = front;
			int currIndex=1;
			while(currNode&& currNode->roomNo!=roomNo)
			{
				currNode=currNode->next;
				currIndex++;
			}
			return currNode;	
		}
		//friend
		friend ostream& operator <<(ostream&,const MeetingRoomQueue &);		
};


//display meeting room queue
//operator << overloading function
ostream& operator<< (ostream &out, const MeetingRoomQueue &rooms){
	int num=0;
	MeetingRoomNode* currNode=rooms.front;
	cout<<endl<<left<<setw(3)<<"No"<<setw(11)<<"Style"<<setw(9)<<"Size"<<"Hourly(RM)"<<endl;
	while(currNode)
	{
		cout<<left<<setw(3)<<currNode->roomNo<<setw(11)<<currNode->style
				<<setw(9)<<currNode->size<<setw(13)<<setprecision(2)<<fixed<<currNode->price<<endl;
		currNode=currNode->next;
		num++;
	}
	cout<<"The total number of meeting room is "<<num<<endl;
	return out;
}


class BookingNode{
	public:
		BookingNode* next;       //pointer pointing to the next node 
		int bookingID;           //booking ID of customer
		int custID;              //customer's ID
		int noPerson;            //number of person for a booking
		double payment;          //total price of a room booking (price * noPerson)
		MeetingRoomNode *room;   //meeting room
		Date bookingDate;        //date of a booking
		Date* currentDate;       //current date

		//return current date
		void getCurrentDate()
		{
			time_t now = time(0);
			tm *ltm = gmtime(&now);
			currentDate = new Date(ltm->tm_mday,ltm->tm_mon+1,ltm->tm_year+1900,ltm->tm_wday);
		}

		//default constructor with default arguments
		BookingNode(int bookingID=0,int custID=0,int noPerson=0,double payment=0)
		{
			next=NULL;
			this->bookingID = bookingID;
			this->custID=custID;
			this->noPerson = noPerson;
			this->payment = payment;
			getCurrentDate();
			room = new MeetingRoomNode;		
		}
		
		//set payment
		void setPayment(double payment)
		{
			this->payment = payment;
		}
		//set the room pointer
		void setRoom(MeetingRoomNode * room){
			this->room=room;
		}
		//return true is the room is occupied else return false
		bool check_if_occupied()
		{
			if(room->getOccupied(bookingDate.getDD(),bookingDate.getTimeSlot()-1)=="X")
				return true;
			else
				return false;		
		}
		//set the room with the day and timeslot represented by i and j to occupied
		void setRoomOccupied(int i,int j)
		{
			room->setOccupied(i, j);
		}
		//set the room of the booking date to unoccupied
		void setRoomUnoccupied(){
			room->setUnoccupied(bookingDate.getDD(),bookingDate.getTimeSlot()-1);
		}
		
		//return booking IDfor a booking
		int getBookingID() const{return bookingID;}
		
		//return customer ID for a booking
		int getCustomerID() const{return custID;}
		
		//return number of person for a booking
		int getNoPerson() const{return noPerson;}
		
		//return payment for a booking
		double getPayment() const{return payment;}
		
		//display a table showing the availability of a selected room within 7 days from today according to 10  time slots
		void checkAvailability()
		{
			cout<<"Today's date: "<<currentDate->getDate()<<endl;
			int index = currentDate->getDD();
			cout<<"Today is "<< DAY[index]<<"."<<endl;
			cout<<"Booking can be made within 6 days starting from tomorrow."<<endl;
			cout<<"Each session is 1 hour."<<endl;
			cout<<"_ represents available slot, X represents occupied slot."<<endl<<endl;
			cout<<setw(28)<<" "<<"Meeting Room No "<<room->getRoomNo()<<endl;
			cout<<left<<setw(10)<<" "<<setw(6)<<"09:00"<<setw(6)<<"10:00"<<setw(6)<<"11:00"<<setw(6)<<"12:00"<<setw(6)
			<<"13:00"<<setw(6)<<"14:00"<<setw(6)<<"15:00"<<setw(6)<<"16:00"<<setw(6)<<"17:00"<<setw(6)<<"18:00"<<endl;
			
			if(index == 0)	// today is Sunday
			{
				for(int i=0;i<7;i++)
				{
					cout<<left<<setw(9)<<DAY[i]<<"   ";
					for(int j=0;j<10;j++)
					{
						if(room->getOccupied(i,j)=="_")	//if the timeslot is empty
							cout<<setw(6)<<"_";
						else
							cout<<setw(6)<<"X";
					}
					cout<<endl;
				}					
			}
			else 	//  today is not Sunday
			{
				while(index > 0 && index <= 6)
				{
					for(;index<7;index++)
					{
						cout<<left<<setw(9)<<DAY[index]<<"   ";
						for(int j=0;j<10;j++)
						{
							if(room->getOccupied(index,j) == "_")	//if the timeslot is empty
								cout<<setw(6)<<"_";
							else
								cout<<setw(6)<<"X";
						}
						cout<<endl;	
					}		
					index++;
				}
				index = currentDate->getDD();
				{
					for(int i=0;i<index;i++)
					{
						cout<<left<<setw(9)<<DAY[i]<<"   ";
						for(int j=0;j<10;j++)
						{
							if(room->getOccupied(i,j) == "_")	//if the timeslot is empty
								cout<<setw(6)<<"_";
							else
								cout<<setw(6)<<"X";
						}
						cout<<endl;	
					}	
				}
			}
	    }			
};

class BookingQueue{
	private:
		BookingNode* front;	//pointer pointing to the first node in Booking queue
		BookingNode* rear;  //pointer pointing to the last node in Booking queue
	
	int exchangeNode(int ID)
	{
		BookingNode *currNode = front;
		BookingNode *prevNode;
		int currIndex = 1;
		//Find the Booking Node with the same booking ID
		while(currNode && currNode->bookingID != ID)
		{
			prevNode = currNode;
			currNode = currNode->next;
			currIndex++;
		}
		
		if(currNode == front)
			return 1;
		
		if(currNode)
		{
			BookingNode *temp = front;
			front = currNode;
			prevNode->next = temp;
			
			//Exchange the next pointer
			BookingNode *node = currNode->next;
			currNode->next = temp->next;
			temp->next = node;	
			
			return currIndex;
		}
		return 0;		
	}
	
	//Sort the queue linked list 
	void sortQueue()
    {  
	    BookingNode *currNode = front, *index = NULL;    
	      
	    while(currNode) {  
	        //Node index will point to node next to currNode 
	        index = currNode->next;  
	          
	        while(index)
		    {  
	            //If current node's data is greater than index's node data, swap the data between them  
	            if(currNode->getBookingID() > index->getBookingID()) 
				{  
	                int temp1 = currNode->bookingID;  
	                int temp2 = currNode->custID;
	                int temp3 = currNode->noPerson;
	                double temp4 = currNode->payment;
	                MeetingRoomNode *temp5 = currNode->room;
	                Date temp6 = currNode->bookingDate;
	                
	                currNode->bookingID = index->bookingID;
	                currNode->custID = index->custID;
	                currNode->noPerson = index->noPerson;
	                currNode->payment = index->payment;
	                currNode->room = index->room;
	                currNode->bookingDate = index->bookingDate;
	                  
	                index->bookingID = temp1;
	                index->custID = temp2;
	                index->noPerson = temp3;
	                index->payment = temp4;
	                index->room = temp5;
	                index->bookingDate = temp6;
	            }  
	            index = index->next;  
	        }  
	        currNode = currNode->next;  
	    }      
    }  
      
	public:
		//constructor
		BookingQueue()
		{
			front = rear = NULL;	//The Booking queue is initially empty, set front and rear pointer to NULL
		}
		//destructor
		~BookingQueue()
		{
			BookingNode* temp = front;
			while(temp)
			{
				front = temp->next;
				delete temp;
				temp = front;
			}
		}
		//return true if the Booking queue is empty else return false
		bool isEmpty_booking()
		{
			return front == NULL && rear == NULL;
		}
		//insert Booking node to the rear of Booking queue
		BookingNode* enQueue_booking(int bookingID=0,int custID=0,int noPerson=0,double payment=0)
		{
			//Create a new booking node
			BookingNode* newNode=new BookingNode(bookingID,custID,noPerson,payment);
			
			//Insert to an empty Booking queue
			if(isEmpty_booking())
			{
				front = rear = newNode;
			}
			//Insert to a non-empty Booking queue
			else
			{
				newNode->next = NULL;
				rear->next = newNode;
				rear = newNode;
			}
			
			return newNode;
		}
		//Book a room
		void book(MeetingRoomQueue& roomQueue,int roomNo, int bookingID, int custID)
		{
			BookingNode* newNode;
			int noPerson;
			double payment;
			cout<<"Number of people involved in the meeting: ";
			cin>>noPerson;

			newNode=enQueue_booking(bookingID, custID, noPerson);
			newNode->room=roomQueue.assignRoom(roomNo);
			payment = (newNode->room)->getPrice() * noPerson;
			newNode->setPayment(payment);
			(newNode->bookingDate).readBookingDate();
			while(newNode->check_if_occupied()==true)
			{
				cout<<"The selected date and timeslot was occupied."<<endl;
				cout<<"Please enter the date again."<<endl<<endl;
				(newNode->bookingDate).readBookingDate();
			}
			
			newNode->setRoomOccupied((newNode->bookingDate).getDD(),(newNode->bookingDate).getTimeSlot()-1);
			
			cout<<"\nYour booking ID is "<<bookingID<<endl;
			cout<<"Total Payment: RM"<<setprecision(2)<<fixed<<payment<<endl;
			cout<<"~ You have successfully booked a room ~ "<<endl;
		}
		
		//Delete a Booking node at the front of the Booking queue
		int deQueue_booking(int bookingID)
		{	
			//Change the Booking node with the bookingID to the first node
			int bookIndex = exchangeNode(bookingID);
			
			//Delete from the front of the Booking queue
			BookingNode *temp = front;
			temp->setRoomUnoccupied();
			front = front->next;
			delete temp;
			
			//If the Booking queue contains only one Booking node
			if(front == NULL)
				rear = NULL;
			
			sortQueue();
			
			return bookIndex;
		}
		
		//return the position if booking node with bookingID passed as argument is found else return 0
		int findNode(int bookingID)
		{
			BookingNode * currNode = front;
			int currIndex=1;
			//Search the booking node with the matched bookingID and locate its position
			while(currNode&& currNode->bookingID!=bookingID)
			{
				currNode=currNode->next;
				currIndex++;
			}
			//return the position of the booking node with the matched bookingID
			if(currNode)
			{
				return currIndex;
			}
			//return 0 if no booking node is found with the matched bookingID
			else
			{
				return 0;
			}	
		}	
		//return the position if the booking node with matched customer ID and booking ID is found else return 0;
		int checkID(int ID,int bookingID)
		{
			BookingNode *currNode = front;
			int currIndex=1;
			//Search the booking node with the matched bookingID and custID and locate its position
			while(currNode && (currNode->bookingID!=bookingID||currNode->custID!=ID))
			{
				currNode=currNode->next;
				currIndex++;
			}
			//return the position of the booking node with the matched bookingID and custID
			if(currNode)
			{
				return currIndex;
			}
			//return 0 if no booking node is found with the matched bookingID and custID
			else
			{
				return 0;
			}	
		}
		
		void printBookingDetails(int ID) const
		{
			BookingNode *currNode = front;
			int currIndex;
			double total;
			while(currNode)
			{
				if(currNode->custID == ID)
				{
					cout<<"Booking ID: "<<currNode->getBookingID()<<endl;
					cout<<"Meeting Room Number: "<<currNode->room->getRoomNo()<<endl;
					cout<<"Meeting Room Size  : "<<currNode->room->getSize()<<endl;
					cout<<"Meeting Room Style : "<<currNode->room->getStyle()<<endl;
					cout<<"Booking Date	   : "<<currNode->bookingDate.getDate()<<endl;
					cout<<"Booking Day	   : "<<DAY[currNode->bookingDate.getDD()]<<endl;
					cout<<"Booking TimeSlot   : "<<TIMESLOT[currNode->bookingDate.getTimeSlot()-1]<<endl;
					cout<<"Total Payment      : RM"<<fixed<<setprecision(2)<<currNode->getPayment()<<endl<<endl;
					total+=currNode->getPayment();
					currIndex++;
				}
				currNode = currNode->next;		
			}
			cout<<"Total Payment for All Booking(s): RM "<<total<<endl<<endl;
		}
		
};

class CustomerNode{
	public:
		CustomerNode* next;   	//pointer pointing to the next node
		int custID;          	//customer's ID
		string custName;     	//customer's name
		string custIC;       	//customer's IC
		int custAge;         	//customer's age
		string custPhone;    	//customer's phone
		string custAddress;  	//customer's address
		BookingNode* booking;   // customer's booking
		
		//default constructor with default arguments
		CustomerNode(int ID=0,string cn="",string IC="",int age=0,string cphone="",string caddress="")
		{
			next = NULL;
			custID = ID;
			custName = cn;
			custIC = IC;
			custAge=age;
			custPhone = cphone;
			custAddress = caddress;
		}
		
		string getCustomerName() const{return custName;}
					
};

class CustomerQueue{
	private:
		CustomerNode* front;  //pointer pointing to the first node in Customer queue
		CustomerNode* rear;  //pointer pointing to the last node in Customer queue
	public:
		//constructor
		CustomerQueue()
		{
			front = rear = NULL;	//the customer list is initially empty, head is set to NULL
		}
		//destructor
		~CustomerQueue()
		{
			CustomerNode* temp = front;
			while(temp != NULL)
			{
				front = temp->next;
				delete temp;
				temp = front;
			}
		}
		//return true if the Customer queue is empty, else return false
		bool isEmpty_customer()
		{
			return front == NULL && rear == NULL;
		}
		//insert customer node to the rear of the Customer Queue
		void enQueue_customer(int ID=0,string cn="",string IC="",int age=0,string cphone="",string caddress="")
		{
			//Create a new Customer Node
			CustomerNode* newNode = new CustomerNode(ID,cn,IC,age,cphone,caddress);
			
			//Insert to an empty Customer queue
			if(isEmpty_customer())
			{
				front = rear = newNode;
			}
			//Insert to a non-empty Customer queue
			else
			{
				newNode->next = NULL;
				rear->next = newNode;
				rear = newNode;
			}
		}
		//delete Customer node at the front of the Customer queue
		void deQueue_customer()
		{
			//Cannot delete the Customer node when the Customer Queue is empty
			if(isEmpty_customer())
			{
				cout<<"The customer queue is currently empty. No customer to delete.\n";
				return;
			}
			
			//Delete from the front of the Customer queue
			CustomerNode *temp = front;
			front = front->next;
			delete temp;
			
			//If the Customer queue contains only one Customer node
			if(front == NULL)
				rear = NULL;
		}
		//return the position if the node with customer ID passed as argument is found else return 0
		int findNode(int ID)
		{
			CustomerNode * currNode = front;
			int currIndex=1;
			//Search the customer node with the matched custID and locate its position
			while(currNode&& currNode->custID!=ID)
			{
				currNode=currNode->next;
				currIndex++;
			}
			//return the position of the customer node with the matched custID
			if(currNode)
			{
				return currIndex;
			}
			//return 0 if no customer node is found with the matched custID
			else
			{
				return 0;
			}	
		}
		
		//print customer details	
		void printCustDetails(int ID) const
		{
			CustomerNode * currNode = front;
			while(currNode&& currNode->custID!=ID)
			{
				currNode=currNode->next;
			}
			cout<<"Customer ID  : "<<currNode->custID<<endl;
			cout<<"Customer Name: "<<currNode->custName<<endl;
			cout<<"Customer IC  : "<<currNode->custIC<<endl;
			cout<<"Customer Age : "<<currNode->custAge<<endl;
			cout<<"Customer Phone Number: "<<currNode->custPhone<<endl;
			cout<<"Customer Address     : "<<currNode->custAddress<<endl<<endl;
		}	
		
		//display customer list
		void displayCustList() const
		{
			int num=0;
			CustomerNode* currNode = front;
			while(currNode)
			{
				cout<<"Customer ID  : "<<currNode->custID<<endl;
				cout<<"Customer Name: "<<currNode->custName<<endl;
				cout<<"Customer IC  : "<<currNode->custIC<<endl;
				cout<<"Customer Age : "<<currNode->custAge<<endl;
				cout<<"Customer Phone Number: "<<currNode->custPhone<<endl;
				cout<<"Customer Address     : "<<currNode->custAddress<<endl<<endl;
				currNode=currNode->next;
				num++;
			}
			cout<<"The total number of customers is "<<num<<endl;
		}	
		
	  string getCustomerName(int ID) const
	  {
	  		CustomerNode * currNode = front;
	  		while(currNode && currNode->custID!=ID)
			{
				currNode=currNode->next;
			}
			return currNode->getCustomerName();
	  }	
};

class Admin{
	private:
		string name;            //admin's name
		int age;                //admin's age
		string phone;           //admin's phone number
		string address;        //admin's address

	public:
		//default constructor with default argument
		Admin(string name="",int age=0,string phone="",string address="")
		{
			this->name=name;
			this->age=age;
			this->phone=phone;
			this->address=address;
		}
		//set admin's name
		void setName(string name)
		{
			this->name=name;
		}
		//set admin's age
		int setAge(int age)
		{
			this->age=age;
		}
		//set admin's phone number
		void setPhone(string phone)
		{
			this->phone=phone;
		}
		//set admin's address
		void setAddress(string address)
		{
			this->address=address;
		}
		//return admin's name
		string getName() const{return name;}
		//return admin's age
		int getAge() const{return age;}	
		//return admin's phone number
		string getPhone() const{return phone;}
		//return admin's address
		string getAddress() const{return address;}	
};

//display main menu, ask the user whether he will continue to use the system or quit the system
int mainMenu()
{
	int choice;
	cout<<"\t\tMeeting Room Booking System\n\n";
	cout<<"~~~~~~~~~~~~~~~~ Welcome to the main menu ~~~~~~~~~~~~~~~~~"<<endl
		<<"1. Continue to use the meeting room booking system."<<endl
		<<"2. Quit system now."<<endl<<endl;
		
	cout<<"Please select your choice (1/2) : ";
	cin>>choice;
	while( choice < 1 ||choice > 2 || cin.fail() )    //validate input
	{   cin.clear();
	    cin.ignore(100,'\n');
		cout<<"Please select your choice (1/2) : ";
		cin>>choice;
	}
	
	return choice;		
}

//display admin menu, prompt user to enter choice
int adminMenu() 
{
	int choice;
	
	cout<<"\t\tMeeting Room Booking System\n\n";
	cout<<"~~~~~~~~~~~~~~~~ Welcome to the Admin Menu ~~~~~~~~~~~~~~~~"<<endl
		<<"1.Display all the customers' details'. "<<endl
		<<"2.View a customer's details based on customer ID."<<endl
		<<"3.Check whether the meeting room is available."<<endl	
		<<"4.Cancel a booking."<<endl
		<<"5.Generate the receipt."<<endl
		<<"6.Quit our system."<<endl<<endl;
	
	cout<<"Please select your choice: ";	
	cin>>choice;
	while(choice < 1 || choice > 6 || cin.fail() ) //validate input
	{   cin.clear();
	    cin.ignore(100,'\n');
		cout<<"Please select your choice: ";
		cin>>choice;
	}
	
	return choice;
}

//display customer menu, prompt user to enter choice
int custMenu() 
{
	int choice;
	
	cout<<"\t\tMeeting Room Booking System\n\n";
	cout<<"~~~~~~~~~~~~~~~ Welcome to the Customer Menu ~~~~~~~~~~~~~~~"<<endl
		<<"1.View all the meeting rooms."<<endl
		<<"2.Check whether the meeting room is available."<<endl
		<<"3.Book a meeting room."<<endl
		<<"4.Cancel a booking."<<endl
		<<"5.Generate the receipt."<<endl
		<<"6.Quit our system."<<endl<<endl;
		
	cout<<"Please select your choice: ";
	cin>>choice;
	while(choice < 1 || choice > 6 || cin.fail() ) //validate input
	{   cin.clear();
	    cin.ignore(100,'\n');
	    cout<<"Please select your choice: ";
		cin>>choice;
	};
		
	return choice;
}

//read admin's information from Admin.txt which is stored in admin variable
void readAdmin(Admin &admin)
{
	string name,phone,address;
	int age;
	ifstream input("Admin.txt");
	if(!input)
	{
		cout<<"Error! Cannot open file."<<endl;
		return;
	}
	else
	{
		getline(input,name);
		input>>age;
		input.ignore();
		getline(input,phone);
		getline(input,address);
		admin.setName(name);
		admin.setAge(age);
		admin.setPhone(phone);
		admin.setAddress(address);
	}
	
	input.close();
}

//read customers' information from CustomerDetails.txt which are stored in custQueue
void readCustomers(CustomerQueue &custQueue)
{
	string name,IC,phone,address;
	int ID,age;
	
	ifstream input("CustomerDetails.txt");
	if(!input)
	{
		cout<<"Error! Cannot open file."<<endl;
		exit(1);
	}
	else
	{
		while(input>>ID)
		{
			input.ignore();
			getline(input,name);
			input>>age;
			input.ignore();
			getline(input,IC);
			getline(input,phone);
			getline(input,address);
			
			custQueue.enQueue_customer(ID,name,IC,age,phone,address);
		}
	input.close();
	}
}

//read the list of meeting room from MeetingRoom.txt which is then stored in roomQueue
void readMeetingRoom(MeetingRoomQueue & roomQueue)
{
	int roomNo;
	string style, size;
	double price;
	
	ifstream input("MeetingRoom.txt");
	if(!input)
	{
		cout<<"Error! Cannot open file."<<endl;
		exit(1);
	}
	for(int i=0;i<15;i++)
	{
		input>>roomNo;
		input>>style;
		input>>size;
		input>>price;
		roomQueue.enQueue_room(roomNo, style, size, price);
	}
	input.close();
}

// return true if the date entered is within six days starting from tomorrow else it will return false
bool withinSixDays(int d,int m,int y)
{
	time_t today=time(0);
	struct tm *ltm = gmtime(&today);
	ltm->tm_sec=0;		ltm->tm_min=0;		ltm->tm_hour=0;
	struct tm bookDate = {0,0,0,d,m-1,y-1900};
	int difference = ceil(difftime(mktime(&bookDate),today)/(60*60*24));
	if(difference >=1 && difference <=6)
		return true;
	else
		return false;
}

//prompt the user to select room number
int selectRoom()
{
	int choice;
	cout<<"\nPlease select the number of meeting room: ";
	cin>>choice;
	while(choice<1 || choice>15||cin.fail())  //validate user's input
	{   
		cin.clear();
	    cin.ignore(100,'\n');
		cout<<"\nPlease select the number of meeting room: ";
		cin>>choice;
		cout<<endl;
	}
	
	return choice;
}

int main()
{
		Admin admin;                 	  //admin
		CustomerQueue custQueue;     	  //customer queue
		MeetingRoomQueue roomQueue;       //room queue
		BookingQueue bookQueue;           //booking queue
		BookingNode* book;                //booking node
		string password;                  //password of admin
		int custID,roomNo,searchID,bookingIndex;
		int bookNo=0;  
		char user,ans;                    //identity of user and answer of user whether to continue a loop
		int mainchoice,choice;       	  //choice of user
		readMeetingRoom(roomQueue);       //read a list of meeting room from file and store into room queue
		readCustomers(custQueue);  		  //read a list of customer from file into customer queue 
		do
		{
			mainchoice = mainMenu();
			if(mainchoice == 2)				//if mainchoice is 2, then quit the whole system
			{
				return 0;
			}
			cout<<"\nPlease select your role in the meeting room booking system (A - Admin / C - Customer) : "; //user identification
			cin>>user;
			cin.ignore();
			
			if(toupper(user) == 'A')   //admin
			{
				cout<<"Please log in by entering your password: "; 
				cin>>password;
				while(password != "12345")   //validate admin's password
				{
					cout<<"You have entered an invalid password."<<endl;
					cout<<"Please enter your password again: ";
					cin>>password;
				}
				readAdmin(admin); //read admin's information from file into admin variable
				system("CLS");
				do
				{
					choice = adminMenu();
					system("CLS");
					cout<<"~~~~~~~~~~~~~~~ Admin Menu ~~~~~~~~~~~~~~~\n\n";
					switch(choice)
					{					 
						case 1:     cout<<"========== Viewing Customer List ==========\n\n";
									if(!custQueue.isEmpty_customer())	   //view a customer list		
										custQueue.displayCustList();
									else
										cout<<"Sorry. There is no customer booking a room yet. Please select other options."<<endl;
									break;
									
						case 2: 	cout<<"====== Viewing A Customer's Details ======\n\n";
									if(custQueue.isEmpty_customer())    //view a customer's details based on customer ID
										cout<<"Sorry. There is no customer booking a room yet. Please select other options."<<endl;
									else
									{
										cout<<"Please enter the customer ID to view the customer's details: ";
										cin>>searchID;
										while(cin.fail()||!custQueue.findNode(searchID)) 
										{	
											cout<<"Please enter the customer ID to view the customer's details: ";
											cin>>searchID;
										}
										custQueue.printCustDetails(searchID);		
									}
									break;
											
						case 3:		cout<<"==== Check Availability of Meeting Room ====\n\n";
									cout<<roomQueue; 			//check whether the meeting room is available	
									roomNo = selectRoom();	
									book=new BookingNode;
									book->setRoom(roomQueue.assignRoom(roomNo));							
									book->checkAvailability();
									delete book;
									book=NULL;
									break;
											
						case 4: 	cout<<"============= Cancel Booking =============\n\n";
									if(bookQueue.isEmpty_booking())    //cancel a booking
									{
										cout<<"Sorry.There is no booking made by a customer yet.Please select other options."<<endl;
									}
									else
									{
										cout<<"Please enter the customer's booking ID: ";
										cin>>bookingIndex;
										int loop1 = 1;
										while(!bookQueue.findNode(bookingIndex) && loop1)
										{
											cout<<"Booking ID cannot be found."<<endl;
											cout<<"Do you want to continue to cancel the customer's booking? (Y/N): ";
											cin>>ans;
											while(toupper(ans) != 'Y' && toupper(ans) != 'N')
											{
												cout<<"Invalid answer.Please enter Y or N only."<<endl;
												cout<<"Do you want to continue to cancel the customer's booking? (Y/N): ";
												cin>>ans;
											}
											if(toupper(ans) == 'N')
											{
												loop1 = 0;
												break;
											}	
											cout<<"Please reenter the customer's booking ID: ";	
											cin>>bookingIndex;
										}
										if(loop1)
										{
											bookQueue.deQueue_booking(bookingIndex);
											cout<<"The customer's booking is successfully cancelled"<<endl;
										}
									}
									break;
									
						case 5:		cout<<"============== Receipt Generation ==============\n\n";
									if(bookQueue.isEmpty_booking())
									{
										cout<<"Sorry. We are enable to generate a receipt since you have not made a booking yet."<<endl;
									}
									else
									{
										cout<<"Enter the customer ID: ";
										cin>>custID;
										while(cin.fail()|| !custQueue.findNode(custID))
										{   
											cout<<"Invalid Customer ID. Please reenter the customer ID: ";
											cin.clear();
										    cin.ignore(100,'\n');
										    cin>>custID;
										}	   
										cout<<"~~~~~~~~~~~~~~~~~~~ RECEIPT ~~~~~~~~~~~~~~~~~~~~"<<endl;
										cout<<"Name: "<<custQueue.getCustomerName(custID)<<endl;
										bookQueue.printBookingDetails(custID);
										cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
									}
									
												
						case 6: 	cout<<"============== Quit Admin Menu ==============="<<endl<<endl; //quit admin menu
					}
					cout<<endl;
					system("Pause");
					system("CLS");
				}while(choice != 6);
			}
			else if(toupper(user) == 'C')    //customer
			{
				cout<<"\nPlease enter your customer ID: ";				//verify customer's ID
				cin>>custID;
				while(cin.fail()||!custQueue.findNode(custID))
				{   
					cout<<"Invalid Customer ID. Please reenter your customer ID: ";
					cin.clear();
				    cin.ignore(100,'\n');
				    cin>>custID;
				}
				system("CLS");
				do
				{
					choice = custMenu();
					system("CLS");
					cout<<"~~~~~~~~~~~~~~~ Customer Menu ~~~~~~~~~~~~~~~\n\n";
					switch(choice)
					{					  
						case 1: 	cout<<"============ Viewing Meeting Room ============\n\n";
									cout<<roomQueue; //view all meeting room with their details
									break;
									      
						case 2:		cout<<"===== Checking Availability of Meeting Room ======\n\n";
									cout<<roomQueue;  //check whether the meeting room is available	
									roomNo = selectRoom();	
									book=new BookingNode;
									book->setRoom(roomQueue.assignRoom(roomNo));								
									book->checkAvailability();
									delete book;
									book=NULL;
									break;
										
						case 3: 	cout<<"============= Making A Booking ==============\n";
									roomNo = selectRoom();   //book a meeting room
									++bookNo;								
									bookQueue.book(roomQueue,roomNo, bookNo, custID);
									break;
									
						case 4:		cout<<"============== Cancel A Booking ==============\n\n";
									if(bookQueue.isEmpty_booking())    //cancel a booking
									{
										cout<<"Sorry.There is no booking made by a customer yet.Please select other options."<<endl;
									}
									else
									{   
										cout<<"Please enter your booking ID: ";
										cin>>bookingIndex;
										int loop2 = 1;
										while(!bookQueue.checkID(custID,bookingIndex) && loop2)
										{
											cout<<"Customer ID is not matched or Booking ID is not found."<<endl;
											cout<<"Do you want to continue to cancel the booking? (Y/N): ";
											cin>>ans;
											while(toupper(ans) != 'Y' && toupper(ans) != 'N')
											{
												cout<<"Invalid answer.Please enter Y or N only."<<endl;
												cout<<"Do you want to continue to cancel the booking? (Y/N): ";
												cin>>ans;
											}
											if(toupper(ans) == 'N')
											{
												loop2 = 0;
												break;
											}
													
											cout<<"Please reenter your booking ID: ";	
											cin>>bookingIndex;
										}
										if(loop2)
										{
											bookQueue.deQueue_booking(bookingIndex);
											cout<<"Your booking is successfully cancelled"<<endl;
										}
									}
									break;	
									
						case 5: 	cout<<"============== Receipt Generation ==============\n\n";
									if(bookQueue.isEmpty_booking())
									{
										cout<<"Sorry. We are enable to generate a receipt since you have not made a booking yet."<<endl;
									}
									else
									{
										cout<<"~~~~~~~~~~~~~~~~~~~ RECEIPT ~~~~~~~~~~~~~~~~~~~~"<<endl;
										cout<<"Name: "<<custQueue.getCustomerName(custID)<<endl;
										bookQueue.printBookingDetails(custID);
										cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
									}
									break;
									   
						case 6: 	cout<<"=== Quit Customer Menu! Thank you for using our meeting room booking system! ==="<<endl;  //quit customer menu
					}
					cout<<endl;
					system("Pause");
					system("CLS");
				}while(choice != 6);
			}
		}while(mainchoice == 1);
		return 0;
}
 


