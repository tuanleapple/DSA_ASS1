#include "main.h"

class imp_res : public Restaurant
{
private:
	customer* lastChange;
	int count = 0;

	//Shellsort only
	int swapCount = 0;

#pragma region customSubClasses
	//TODO Queue
	class QueueModified {
	public:
		class Node;
	private:
		Node* head;
		Node* tail;
		int count;

		int joinTime;
	public:

		QueueModified() {
			this->head = this->tail = nullptr;
			this->count = this->joinTime = 0;
		}
		~QueueModified(){
			while (count) removeAt(0);
		}

		bool push(customer* target, bool isInTable = false) {
			customer* cus = new customer(target->name, target->energy, nullptr, nullptr);
			if (count == 2 * MAXSIZE) return false;
			if (!head) {
				head = tail = new Node(cus, joinTime++);
			}
			else {
				tail->next = new Node(cus, joinTime++);
				tail = tail->next;
			}

			tail->isInTable = isInTable;

			count++;
			return true;
		}//Push deep copy

		bool pop() {
			if (!head) return false;

			Node* delNode = head;
			head = head->next;

			delNode->next = nullptr;
			delete delNode;
			count--;
			return true;
		}
		bool removeAt(int index) {
			if (index < 0 || index >= count) return false;

			if (index == 0) return pop();//Delete head

			Node* p = head;
			Node* pPre = nullptr;
			int i = 0;
			while (i < index) {//Delete mid
				pPre = p;
				p = p->next;
				i++;
			}
			if (pPre) {
				pPre->next = p->next;
			}

			if (index == count - 1) tail = pPre;//Delete tail

			p->next = nullptr;
			delete p;
			count--;

			return true;
		}
		bool removeItem(customer* target) {
			if (!head) return false;
			if (head->data->name == target->name) return pop();//Delete head

			Node* p = head;
			Node* pPre = nullptr;
			int i = 0;
			while (i < count && p->data->name != target->name) {//Delete mid
				pPre = p;
				p = p->next;
				i++;
			}

			if (!p) return 0; //DOMAIN EXPANSION COUNTER
			if (pPre) {
				pPre->next = p->next;
			}

			if (i == count - 1) tail = pPre;//Delete tail

			p->next = nullptr;
			delete p;
			count--;

			return true;
		}
		void clear() {
			count = joinTime = 0;
			while (count) pop();
		}

		Node* front() {
			return head;
		}
		int indexOf(string name) {
			Node* p = head;
			int i = 0;
			while (p && i < count) {
				if (p->data->name == name) return i;
				i++;
				p = p->next;
			}
			return -1;
		}
		Node* get(int index) {
			Node* p = head;
			int i = 0;
			while (p && i < index) {
				i++;
				p = p->next;
			}
			return p;
		}
		Node* get(customer* target) {
			Node* p = head;
			int i = 0;
			while (p->data->name != target->name && i < count) {
				i++;
				p = p->next;
			}
			return p;
		}


		int getSize() {
			return count;
		}
		bool empty() {
			return count == 0;
		}

		//PURPLE
		void swap(Node* nodei, Node* nodej) {//Only swap data
			customer* cusTemp = nodei->data;
			nodei->data = nodej->data;
			nodej->data = cusTemp;

			//:)))
			int jt = nodei->joinTime;
			nodei->joinTime = nodej->joinTime;
			nodej->joinTime = jt;
		}

		//TIMEQUEUE ONLY
		void blueHelper(int num, imp_res* restaurant) {
			Node* p = head;
			for (; p != nullptr && num;) {
				if (p->isInTable) {
					Node* toDelete = p;
					
					restaurant->tableRemove(toDelete->data);
					p = p->next;
					this->removeItem(toDelete->data);

					--num;
				}
				else p = p->next;
			}
		}

		//PURPLE SORT
		int findMaxAbsIndex() {
			Node* p = head;
			int resIndex = 0;
			Node* resNode = head;

			for (int i = 0; i < count; i++, p = p->next) {
				int resE, pE;
				resE = resNode->data->energy;
				pE = p->data->energy;

				if (abs(resE) <= abs(pE)){
					bool choose = false;
					if (abs(resE) == abs(pE)){
						if (resNode->joinTime < p->joinTime)
							choose = true;
					}
					else choose = true;

					if (choose){
						resNode = p;
						resIndex = i;
					}
				}
			}

			return resIndex;
		}

		//DOMAINEXPANSION
		//TIMEQUEUE ONLY
		int energySum(bool positive = true) {
			//positive -> calculate sum of sorcerers
			int sum = 0;

			int one = -1;
			if (positive) one = 1;

			for (Node* p = head; p != nullptr; p = p->next)
				if (one * p->data->energy > 0) sum += p->data->energy;
			
			return sum;
		}
		//REMOVE BASED ON TIMEQUEUE
		void domain_expansion(Node* p, bool positive, 
			imp_res* restaurant, QueueModified* waitQueue) {
			//positive -> remove sorcerers
			if (!p) return;

			//Save before del
			Node* pAdvance = p->next;
			customer newCus(p->data->name, p->data->energy, nullptr, nullptr);

			int one = 1;
			if (positive) one = -1;

			if (one * p->data->energy < 0) {

				//Remove customer from all containers
				if (p->isInTable) restaurant->tableRemove(p->data);
				else waitQueue->removeItem(p->data);
				this->removeItem(p->data);
				domain_expansion(pAdvance, positive, restaurant, waitQueue);
				//Print customer
				newCus.print();
			}
			else
			domain_expansion(pAdvance, positive, restaurant, waitQueue);
		}

		//LIGHT
		void print(){
			for (Node* p = head; p != nullptr; p = p->next) {
				p->data->print();
			}
		}
	public:
		class Node {
		private:
			customer* data;
			Node* next;
			int joinTime;

		public:
			bool isInTable;
			friend class QueueModified;
			Node(customer* data, int joinTime = 0, Node* next = nullptr) {
				this->data = data;
				this->joinTime = joinTime;
				this->next = next;
				isInTable = false;
			}
			~Node(){ delete data; next = nullptr;}
			customer* getData() {
				return data;
			}
			int getJoinTime() {
				return joinTime;
			}

			bool operator<(Node* other) {
				if (abs(other->data->energy) == abs(this->data->energy)) {
					return this->joinTime > other->joinTime;
				}
				else return abs(this->data->energy) < abs(other->data->energy);
			}

		};
	};
#pragma endregion

	QueueModified* waitQueue;
	QueueModified* timeQueue;

public:	
	imp_res() {
		lastChange = nullptr;
		waitQueue = new QueueModified();
		timeQueue = new QueueModified();
	};
	~imp_res(){
		tableClear();
		delete waitQueue;
		delete timeQueue;
	}

	customer* findHighRES(int energy) {
		//FROM LAST CHANGE POSITION
		customer* CusHighRES = lastChange;

		int maxRES = abs(energy - lastChange->energy);
		int i = 0;

		for (customer* p = lastChange; i < count; ++i, p = p->next) {
			int diff = abs(p->energy - energy);//FORUM
			if (maxRES < diff) {
				maxRES = diff;
				CusHighRES = p;
			}
		}

		return CusHighRES;
	}
	void addToTable(customer* cus) {
		timeQueue->get(cus)->isInTable = true;

		if (!lastChange) { // Table empty
			lastChange = cus;
			lastChange->next = lastChange->prev = lastChange; //Cycle
		}
		else {
			if (count >= int(MAXSIZE / 2)) lastChange = findHighRES(cus->energy);

			if (cus->energy >= lastChange->energy) { //Add right
				cus->next = lastChange->next;
				cus->prev = lastChange;

				lastChange->next->prev = cus;
				lastChange->next = cus;
			}
			else {	//Add left
				cus->prev = lastChange->prev;
				cus->next = lastChange;

				lastChange->prev->next = cus;
				lastChange->prev = cus;
			}
		}

		count++;
		lastChange = cus;

		//DEBUG
		//cout << lastChange->prev->name << "<-" << lastChange->name << "->" << lastChange->next->name << endl;
	}
	void RED(string name, int energy){
		if (energy == 0) return;//Deny normies
		if (waitQueue->getSize() == MAXSIZE) return;//Fully full
		if (timeQueue->indexOf(name) != -1) return;

		customer* cus = new customer(name, energy, nullptr, nullptr);

		timeQueue->push(cus, count < MAXSIZE);

		if (count == MAXSIZE) {
			waitQueue->push(cus);
			delete cus;
			return;
		}

		addToTable(cus);
	}

	void tableClear(){
		while (count--){
			customer* toDel = lastChange;
			lastChange = lastChange->next;

			toDel->next = toDel->prev = nullptr;
			delete toDel;
		}
	}
	bool tableRemove(customer* target){
		if (!lastChange) return false;
		
		customer* p = lastChange;
		int i = 0;

		for (int i = 0; i < count; ++i, p = p->next) {
			if (p->name == target->name) break;
		}

		if (p) {
			count--;
			//SET NEW LASTCHANGE

			if (p->energy > 0) lastChange = p->next;
			else lastChange = p->prev;

			if (p->next) p->next->prev = p->prev;
			if (p->prev) p->prev->next = p->next;
			p->next = p->prev = nullptr;
			delete p;

			if (!count) lastChange = nullptr;

			return true;
		}
		return false;//DOMAIN EXPANSION COUNTER
	}
	void BLUE(int num)
	{
		if (!num) return;
		timeQueue->blueHelper(num, this);

		while (!waitQueue->empty() && count < MAXSIZE) {
			customer* p = waitQueue->front()->getData();
			customer* cus = new customer(p->name, p->energy, nullptr, nullptr);
			addToTable(cus);//BY PASS RED CHECK
			waitQueue->pop();
		}
	}

#pragma region shellsort

	void inssort2(int start, int n, int incr) {
		for (int i = incr; i < n; i += incr)
			for (int j = i; (j >= incr); j -= incr) {
				//Stop when j-incr <= j
				QueueModified::Node* nodejSub = waitQueue->get(start + j - incr);
				QueueModified::Node* nodej = waitQueue->get(start + j);

				//follow forum, compare abs
				//nodejSub >= nodej -> break;
				if (!nodejSub->operator<(nodej)) break;
				
				// dbg(incr);
				// dbg(to_string(nodejSub->getData()->energy) + "<->" + to_string(nodej->getData()->energy));

				swapCount++;
				waitQueue->swap(nodejSub, nodej);
			}
	}

	int shellSort() {
		swapCount = 0;

		int n = waitQueue->findMaxAbsIndex() + 1;

		for (int i = n / 2; i > 2; i /= 2) // For each increment
			for (int j = 0; j < i; j++) // Sort each sublist
				inssort2(j, n - j, i);
		inssort2(0, n, 1);

		return swapCount;
	}
#pragma endregion
	void PURPLE()
	{
		//
		int N = shellSort();

		//DEBUG
		// N = 0;
		// dbg("-------------------N: " + to_string(N));

		BLUE(N % MAXSIZE);
	}

	void tableSwap(customer* cus1, customer* cus2) {//SWAP next/prev ONLY
		if (cus1 == cus2) return;//Same node

		customer* temp = cus1->prev;
		cus1->prev = cus2->prev;
		cus2->prev = temp;

		cus1->prev->next = cus1;
		cus2->prev->next = cus2;
	
		temp = cus1->next;
		cus1->next = cus2->next;
		cus2->next = temp;

		cus1->next->prev = cus1;
		cus2->next->prev = cus2;
	}
	void tableReverse(bool positive = true) {
		int one = -1;
		if (positive) one = 1;

		customer* pCClock = lastChange;
		customer* pClock = lastChange->next;

		customer* saveLastChange = lastChange;
		int done = 1;
		while (done < count) {
			while (one * pCClock->energy < 0 && done < count) {
				done++;
				pCClock = pCClock->prev;
			}
			while (one * pClock->energy < 0 && done < count) {
				done++;
				pClock = pClock->next;
			}

			if (done >= count) break;

			customer* pCCAdvance = pCClock->prev;
			customer* pCAdvance = pClock->next;

			tableSwap(pCClock, pClock);

			if (pCClock == lastChange) lastChange = pClock;
			else if (pClock == lastChange) lastChange = pCClock;

			//advance after swap:
			done += 2;
			pCClock = pCCAdvance;
			pClock = pCAdvance;
		}

		if (positive) tableReverse(false);
		lastChange = saveLastChange;
	}
	void REVERSAL()
	{
		if (count <= 2) return;
		tableReverse();
	}

	void tableVoidPrint(customer* start, customer* end, int maxLen) {
		customer* pMin = start;
		customer* p = start;

		int minIndex = 0;

		for (int i = 0; i < maxLen; ++i, p = p->next) {
			//Get first equal
			if (p->energy < pMin->energy) {
				minIndex = i;
				pMin = p;
			}
		}

		p = pMin;
		for (int i = 0; i < maxLen; ++i, p = p->next) {
			p->print();
			if (p == end) p = start->prev;
		}
	}
	void UNLIMITED_VOID()
	{
		if (count < 4) return;
		int minSum = INT32_MAX;
		int maxLen = 0;

		int sum = 0;
		int len = 0;
		customer* start, * end;
		start = end = nullptr;

		int i = 0;
		for (customer* pI = lastChange; i < count; ++i, pI = pI->next) {
			sum = len = 0;
			int j = 0;
			for (customer* pJ = pI; j < 2*count; ++j, pJ = pJ->next) {
				if (len >= count) break;
				sum += pJ->energy;
				len++;
				bool choose = false;
				if (len >= 4) {
					if (sum == minSum && len >= maxLen) choose = true;
					if (sum < minSum) choose = true;
					if (choose) {
						start = pI; end = pJ;
						minSum = sum;
						maxLen = len;
					}
				}
			}
		}

		tableVoidPrint(start, end, maxLen);
	}

	void DOMAIN_EXPANSION()
	{
		//Find sum of both sides
		//Remove in all containers according to timeQueue 
		//Add new customers to table if avaiable
		//Print removed in descending joinTime (recursive);

		int sorcerersSum = timeQueue->energySum(true);

		//
		//int spiritsSum = abs(sorcerersSum + timeQueue->energySum(false));
		
		//NEW REQUIREMENT
		int spiritsSum = abs(timeQueue->energySum(false));

		// sorcerer < spirit == true -> remove sorcerer
		timeQueue->domain_expansion(timeQueue->front(), 
			sorcerersSum < spiritsSum, this, waitQueue);

		//copied from num
		while (!waitQueue->empty() && count < MAXSIZE) {
			customer* p = waitQueue->front()->getData();
			customer* cus = new customer(p->name, p->energy, nullptr, nullptr);
			addToTable(cus);//BY PASS RED CHECK
			waitQueue->pop();
		}
	}

	void tablePrint(bool cw) {
		customer* p = lastChange;
		for (int i = 0; i < count; ++i) {
			p->print();
			if (cw) p = p->next;
			else p = p->prev;
		}
	}
	void LIGHT(int num){
		if (num != 0) tablePrint(num > 0);
		else waitQueue->print();
	}
};