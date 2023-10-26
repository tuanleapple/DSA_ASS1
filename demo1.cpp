#include "main.h"

class imp_res : public Restaurant
{
private:
	class Customer
	{
	public:
		string name;
		int energy;
		int timer;
		Customer *next;
		Customer *prev;
		Customer(string na, int e, Customer *p, Customer *ne, int t) : name(na), energy(e), prev(p), next(ne), timer(t) {}
		~Customer() {}
	};
	class waitListQueue
	{
	public:
		Customer *front;
		Customer *rear;
		int count;
		waitListQueue() : front(nullptr), rear(nullptr), count(0) {}

		~waitListQueue()
		{
			while (!isEmpty())
			{
				removeFront();
			}
		}
		bool isEmpty()
		{
			return count == 0;
		}
		int size()
		{
			return count;
		}
		void clear()
		{
			while (!isEmpty())
			{
				removeFront();
			}
		}
		void printWaitList()
		{
			if (front == nullptr)
			{
				cout << "Wait list is empty." << endl;
				return;
			}

			Customer *temp = front;
			do
			{
				cout << temp->name << "-" << temp->energy << "\n";
				temp = temp->next;
			} while (temp != front);
		}

		void insertRear(const string &name, const int &energy, int &timer)
		{
			Customer *newCustomer = new Customer(name, energy, nullptr, nullptr, timer);
			timer++;
			if (isEmpty())
			{
				front = newCustomer;
				front->next = front->prev = front;
			}
			else
			{
				newCustomer->prev = rear;
				newCustomer->next = front;
				rear->next = newCustomer;
				front->prev = newCustomer;
			}
			rear = newCustomer;
			this->count++;
		}
		bool removeFront()
		{
			if (isEmpty())
			{
				return false;
			}

			Customer *temp = front;

			if (front == rear)
			{
				front = rear = nullptr;
			}
			else
			{
				front = front->next;
				front->prev = rear;
				rear->next = front;
			}
			temp->next = nullptr;
			temp->prev = nullptr;
			delete temp;
			count--;

			return true;
		}
		bool isNameInWaitList(const string &name)
		{
			if (isEmpty())
			{
				return false;
			}

			Customer *curr = front;
			do
			{
				if (curr->name == name)
				{
					return true;
				}
				curr = curr->next;
			} while (curr != front);

			return false;
		}
		void swapCustomers(Customer *a, Customer *b)
		{
			if (a == b)
			{
				return;
			}
			Customer *prevA = a->prev;
			Customer *nextA = a->next;
			Customer *prevB = b->prev;
			Customer *nextB = b->next;

			if (nextA == b)
			{
				a->next = nextB;
				b->prev = prevA;

				if (nextB != NULL)
					nextB->prev = a;
				if (prevA != NULL)
					prevA->next = b;

				b->next = a;
				a->prev = b;
			}
			else if (nextB == a)
			{
				swapCustomers(b, a);
			}
			else
			{
				// nodes are not adjacent
				a->next = nextB;
				a->prev = prevB;
				b->next = nextA;
				b->prev = prevA;

				if (nextA != NULL)
					nextA->prev = b;
				if (prevA != NULL)
					prevA->next = b;

				if (nextB != NULL)
					nextB->prev = a;
				if (prevB != NULL)
					prevB->next = a;
			}

			// Update front and rear pointers
			if (front == a)
				front = b;
			else if (front == b)
				front = a;

			if (rear == a)
				rear = b;
			else if (rear == b)
				rear = a;
		}

		bool compareCustomer(Customer *a, Customer *b)
		{
			return abs(a->energy) < abs(b->energy) ||
				   (abs(a->energy) == abs(b->energy) && a->timer > b->timer);
		}
		int getPosition(Customer *customer)
		{
			Customer *curr = front;
			int position = 0;
			do
			{
				if (curr == customer)
				{
					return position;
				}
				curr = curr->next;
				position++;
			} while (curr != front);

			return -1; // Return -1 if the customer is not found in the queue
		}
		Customer *getMaxEnergyCustomer()
		{
			if (isEmpty())
			{
				return nullptr;
			}
			Customer *maxEnergyCustomer = front;
			Customer *curr = front->next;
			do
			{
				if (abs(curr->energy) > abs(maxEnergyCustomer->energy) ||
					(abs(curr->energy) == abs(maxEnergyCustomer->energy)) && curr->timer < maxEnergyCustomer->timer)
				{
					maxEnergyCustomer = curr;
				}
				curr = curr->next;
			} while (curr != front);
			return maxEnergyCustomer;
		}
		Customer *advance(Customer *start, int n)
		{
			Customer *curr = start;
			if (n >= 0)
			{
				for (int i = 0; i < n; i++)
				{
					curr = curr->next;
				}
			}
			else
			{
				for (int i = 0; i < -n; i++)
				{
					curr = curr->prev;
				}
			}
			return curr;
		}
		void insSort(Customer *start, int n, int incr)
		{
			Customer *iCustomer = advance(start, incr);
			for (int i = incr; i < n; i += incr)
			{
				Customer *jCustomer = iCustomer;
				Customer *jPrevCustomer = advance(jCustomer, -incr);
				while (jCustomer != start && compareCustomer(jPrevCustomer, jCustomer))
				{
					swapCustomers(jPrevCustomer, jCustomer);
					jCustomer = jPrevCustomer;
					if (jPrevCustomer != start)
						jPrevCustomer = advance(jCustomer, -incr);
				}
				iCustomer = advance(iCustomer, incr);
			}
		}
		void shellSort(Customer *start, int n)
		{
			for (int i = n / 2; i > 2; i /= 2)
			{
				Customer *sublistStart = start;
				for (int j = 0; j < i; j++)
				{
					insSort(sublistStart, n - j, i);
					sublistStart = sublistStart->next;
				}
			}
			insSort(start, n, 1);
		}
		void sortWaitList_PURPLE()
		{
			Customer *maxEnergyCustomer = getMaxEnergyCustomer();
			int position = getPosition(maxEnergyCustomer);
			shellSort(maxEnergyCustomer, position + 1);
		}
	};

public:
	Customer *recent;
	waitListQueue *WL;
	int count;
	int timer;

	imp_res() : recent(nullptr), count(0), timer(0)
	{
		WL = new waitListQueue();
	};
	~imp_res()
	{
		// Yet to be implemented
		while (!isEmpty())
		{
			removeHere();
		}
		delete WL;
	}

	void clearRes()
	{
		while (!isEmpty())
		{
			removeHere();
		}
	}
	void clear()
	{
		clearRes();
		WL->clear();
	}

	bool isEmpty()
	{
		return count == 0;
	}

	bool isNameInRes(const string &name)
	{
		if (isEmpty())
			return false;
		Customer *curr = recent;
		Customer *begin = recent;
		do
		{
			if (curr->name == name)
			{
				return true;
			}
			// end of loop
			curr = curr->next;
		} while (curr != begin);
		return false;
	}

	bool isNameInWL(const string &name)
	{
		return WL->isNameInWaitList(name);
	}

	bool isNameHere(const string &name)
	{
		return isNameInRes(name) || isNameInWL(name);
	}

	void insertPrev(const string &name, const int &energy) // Need to check count before using
	{
		Customer *newCustomer = new Customer(name, energy, nullptr, nullptr, timer);
		timer++;
		if (isEmpty())
		{
			recent = newCustomer;
			recent->next = recent->prev = recent;
		}
		else
		{
			newCustomer->next = recent;
			newCustomer->prev = recent->prev;
			recent->prev->next = newCustomer;
			recent->prev = newCustomer;
			recent = newCustomer;
		}
		++count;
	}
	void insertNext(const string &name, const int &energy) // Need to check count before using
	{
		Customer *newCustomer = new Customer(name, energy, nullptr, nullptr, timer);
		timer++;
		if (isEmpty())
		{
			recent = newCustomer;
			recent->next = recent->prev = recent;
		}
		else
		{
			newCustomer->next = recent->next;
			newCustomer->prev = recent;
			recent->next->prev = newCustomer;
			recent->next = newCustomer;
			recent = newCustomer;
		}
		++count;
	}
	bool removeHere()
	{
		if (isEmpty())
		{
			return false;
		}
		Customer *temp = recent;
		if (recent->next == recent)
		{
			recent = nullptr;
		}
		else
		{
			recent->next->prev = recent->prev;
			recent->prev->next = recent->next;
			if (temp->energy > 0)
			{
				recent = temp->next;
			}
			else
			{
				recent = temp->prev;
			}
		}
		temp->next = nullptr;
		temp->prev = nullptr;
		delete temp;
		count--;
		return true;
	}
	void RED(string name, int energy)
	{
		if (energy == 0 || isNameHere(name))
		{
			return;
		}
		int MAXSIZE_2 = MAXSIZE / 2;
		if (count < 0 || count > MAXSIZE || WL->count < 0 || WL->count > MAXSIZE)
		{
			cout << "out of range"
				 << "\n";
			return;
		}
		if (count == 0)
		{
			insertNext(name, energy);
			return;
		}
		if (0 < count && count < MAXSIZE_2)
		{
			if (energy >= recent->energy)
			{
				insertNext(name, energy);
			}
			else
			{
				insertPrev(name, energy);
			}
		}
		else if (MAXSIZE_2 <= count && count < MAXSIZE)
		{
			int max_diff = 0;
			int curr_diff = 0;
			Customer *begin = recent;
			Customer *curr = recent;
			do // find max diff
			{
				// do this during each loop
				curr_diff = energy - curr->energy;
				if (abs(curr_diff) > abs(max_diff))
				{
					recent = curr;
					max_diff = curr_diff;
				}
				// end of each loop
				curr = curr->next;
			} while (curr != begin);

			if (curr_diff < 0)
			{
				insertPrev(name, energy);
			}
			else
			{
				insertNext(name, energy);
			}
		}
		else // count == MAXSIZE
		{
			if (WL->count < MAXSIZE)
			{
				WL->insertRear(name, energy, timer);
			}
			else
			{
				return;
			}
		}
	}
	void removeSmallestTimer()
	{
		if (isEmpty())
		{
			return;
		}

		Customer *smallestTimerCustomer = recent;
		Customer *curr = recent->next;

		// Find the customer with the smallest timer
		do
		{
			if (curr->timer < smallestTimerCustomer->timer)
			{
				smallestTimerCustomer = curr;
			}
			curr = curr->next;
		} while (curr != recent);

		// Set recent to the customer with the smallest timer
		recent = smallestTimerCustomer;

		// Remove the customer with the smallest timer
		removeHere();
	}
	void BLUE(int num) // Kick num customer out of restaurant
	{
		// BLUE remove num customer
		if (num <= 0)
			return;
		if (num >= count)
		{
			clearRes();
		}
		else
			for (int i = 0; i < num; i++)
			{
				removeSmallestTimer();
			}

		// BLUE add customer from waitList to Restaurant
		while (count != MAXSIZE && !WL->isEmpty())
		{
			RED(WL->front->name, WL->front->energy);
			WL->removeFront();
		}
	}
	void PURPLE()
	{
		cout << "purple" << endl;
	}
	void REVERSAL()
	{
		cout << "reversal" << endl;
	}
	void UNLIMITED_VOID()
	{
		cout << "unlimited_void" << endl;
	}
	void DOMAIN_EXPANSION()
	{
		cout << "domain_expansion" << endl;
	}
	void LIGHT(int num)
	{
		if (num == 0)
		{
			WL->printWaitList();
		}
		else if (recent != nullptr) // Check if recent is not null
		{
			Customer *curr = recent;
			Customer *begin = recent;
			if (num > 0)
			{
				do
				{
					cout << curr->name << "-" << curr->energy << "\n";
					// end of loop
					curr = curr->next;
				} while (curr != begin);
			}
			else if (num < 0)
			{
				do
				{
					cout << curr->name << "-" << curr->energy << "\n";
					// end of loop
					curr = curr->prev;
				} while (curr != begin);
			}
		}
	}
};