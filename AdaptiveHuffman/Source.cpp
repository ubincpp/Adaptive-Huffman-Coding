#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Node {
	char key;
	Node *pLeft;
	Node *pRight;
	int freq;
	short lvl;
	Node() {
		key = '\0';
		freq = -1;
		lvl = 0;
		pLeft = NULL;
		pRight = NULL;
	}
	Node(char k, Node *pL, Node *pR, int f, short level) {
		key = k;
		pLeft = pL;
		pRight = pR;
		freq = f;
		lvl = level;
	}
};

int initiateTree(Node *&pRoot) {
	if (pRoot == NULL) {
		pRoot = new Node(255, NULL, NULL, 0,0);
		return 0;
	}
	return -1;
}

void search(Node *pRoot, bool &check, char c) //kiểm tra node có ở trong cây không?
{
	if (!pRoot) {
		return;
	}
	if (pRoot->key == c){
		check = true;
		return;
	}
	search(pRoot->pLeft, check, c);
	search(pRoot->pRight, check, c);
}

bool isLeaf(Node *pRoot) { 
	if (!pRoot->pLeft && !pRoot->pRight) {
		return true;
	}
	return false;
}

void takeNode(Node *&pRoot, Node *&pTemp, char c, short level) //chuyển con trỏ đến node bị mất cân bằng (để thay thế)
{
	if (!pRoot) {
		return;
	}
	if (pRoot->key == c && pRoot->lvl == level && isLeaf(pRoot)) {
		pTemp = pRoot;
		return;
	}
	takeNode(pRoot->pLeft, pTemp, c, level);
	takeNode(pRoot->pRight, pTemp, c, level);
}

void takeAddress(Node *pRoot, std::string temp, std::string &temp2, char c) { //lấy mã node có trong cây
	if (!pRoot) {
		return;
	}
	if (pRoot->key == c && isLeaf(pRoot)) {
		temp2 = temp;
	}
	takeAddress(pRoot->pLeft, temp + '0', temp2, c);
	takeAddress(pRoot->pRight, temp + '1', temp2, c);
}

int createNode(Node *&pRoot, char k, int f, short level) {
	pRoot = new Node;
	if (pRoot == NULL) {
		return -1;
	}
	pRoot->key = k;
	pRoot->freq = f;
	pRoot->lvl = level;
	return 0;
}

std::string convertDecimalToBinary(char c) {
	int temp{ c };
	std::string binary;
	while (temp != 0) {
		binary.push_back((temp % 2)+'0');
		temp /= 2;
	}
	std::string trueB;
	if (binary.size() < 8) {
		for (int i{ 0 }; i < 8 - binary.size(); i++) {
			trueB.push_back('0');
		}
	}

	for (int i{ (int)binary.size() -1 }; i >= 0; i--) {
		trueB.push_back(binary[i]);
	}
	return trueB;
}

int pow(int k) {
	int temp{ 1 };
	for (int i{ 0 }; i <= k; i++) {
		if (i == 0) {
			temp *= 1;
			continue;
		}
		temp *= 2;
	}
	return temp;
}

char convertBinaryToDecimal(std::string bit) {
	int temp{ 0 };
	int tbit;
	int k{(int)(bit.size() - 1) };
	for (int i{ 0 }; i < bit.size(); i++) {
		if (bit[i] == '1') {
			tbit = 1;
		}
		else {
			tbit = 0;
		}
		temp += tbit * pow(k);
		--k;
	}
	return static_cast<char>(temp);
}

std::string readDataFromFile(std::string file) {
	std::ifstream fileInput(file);
	if (fileInput.fail()) {
		std::cout << "Failed to open this file!" << std::endl;
		return "";
	}
	std::string temp;
	while (!fileInput.eof()) {
		char line[25000];
		fileInput.getline(line, 25000);
		temp += line;
	}
	return temp;
}

void writeDataToFile(std::string file, std::string data, int mode) {
	std::ofstream fileOutput(file,std::ios::app);
	if (fileOutput.fail()) {
		std::cout << "Cannot open file at " << file << std::endl;
		return;
	}
	if (mode == 0) {
		std::string temp;
		temp = convertDecimalToBinary(data[0]);
		fileOutput << temp;
		return;
	}
	fileOutput << data;
}

int adaptiveHuffmanEncoding(std::string fileOutput, Node *pRoot, char c) {
	bool check{ false };
	search(pRoot, check, c);
	if (!check) {
		std::string temp;
		std::string code;
		takeAddress(pRoot, temp, code, 255);
		writeDataToFile(fileOutput, code, 1);
		code = c;
		writeDataToFile(fileOutput,code, 0);
		return 0;
	}
	else {
		std::string temp;
		std::string code;
		takeAddress(pRoot, temp, code, c);
		writeDataToFile(fileOutput, code, 1);
		return 1;
	}
}

int adaptiveHuffmanDecoding(std::string fileOutput, Node *pRoot, std::string &temp, int mode, int &i) {
	std::string escNode;
	std::string escTemp{ "" };
	takeAddress(pRoot, escTemp, escNode, (char)255);
	if (temp == escNode) {
		return 1;
	}
	if (mode == 3 && temp.size() < 8) {
		i = temp.size() - 1;
		return 2;
	}
	if (temp != escNode && temp.size() > escNode.size()) {
		i = temp.size() - 1;
		return 2;
	}
	if (temp != escNode && temp.size() < 8) {
		return 0;
	}
}


void orderHorizontal(Node *pRoot, Node *pParent, std::vector<Node*> &table) { //duyệt các node xem có node nào có trọng số thấp hơn node đang xét không (ưu tiên các node có mức thấp hơn hoặc bên phía phải hơn)
	if (!pParent) {
		return;
	}
	if (pParent->lvl > pRoot->lvl) {
		return;
	}
	if (pParent == pRoot) {
		table.push_back(pRoot);
		return;
	}
	orderHorizontal(pRoot, pParent->pLeft, table);
	if (pParent->freq < pRoot->freq && isLeaf(pParent)) {
		table.push_back(pParent);
		return;
	}
	orderHorizontal(pRoot, pParent->pRight, table);
	if (pParent->freq < pRoot->freq && isLeaf(pParent)) {
		table.push_back(pParent);
		return;
	}
}

void addNodeLevel(Node *pRoot, int i) { //thêm mức cho các node vừa điểu chỉnh
	if (!pRoot) {
		return;
	}
	if (pRoot) {
		pRoot->lvl += i;
	}
	addNodeLevel(pRoot->pLeft, i);
	addNodeLevel(pRoot->pRight, i);
}

void subNodeLevel(Node *pRoot, int i) { //giảm mức các node vừa điều chỉnh
	if (!pRoot) {
		return;
	}
	if (pRoot) {
		pRoot->lvl -= i;
	}
	subNodeLevel(pRoot->pLeft, i);
	subNodeLevel(pRoot->pRight, i);
}


int balanceNode(Node *&pRoot, Node *&pParent) { //cân bằng node vừa được thêm vào cây
	if (!pRoot->pLeft) {
		++pRoot->freq;
	}
	else {
		pRoot->freq = pRoot->pLeft->freq + pRoot->pRight->freq;
	}
	Node *pTemp{ pRoot };
	std::vector<Node*> table;
	orderHorizontal(pTemp, pParent, table);
	for (int i{ 0 }; i < table.size(); i++) {
		if (pTemp->lvl > table[i]->lvl) {
			pTemp = table[i];
		}
	}
	int count{ 0 };
	std::vector<Node*> lTable;
	for (int i{ 0 }; i < table.size(); i++) {
		if (table[i]->lvl == pTemp->lvl) {
			lTable.push_back(table[i]);
		}
	}
	
	for (int i{ 0 }; i < lTable.size(); i++) {
		pTemp = lTable[i];
	}
	if (pTemp == pRoot) {
		return 1;
	}
	if (pTemp->lvl < pRoot->lvl) {
		int sub{ pRoot->lvl - pTemp->lvl };
		addNodeLevel(pTemp, sub);
		subNodeLevel(pRoot, sub);
	}
	Node *pA{ new Node(pRoot->key,pRoot->pLeft,pRoot->pRight,pRoot->freq,pRoot->lvl) }; //thủ tục swap 2 node mất cân bằng
	takeNode(pParent, pTemp, pTemp->key, pTemp->lvl);
	delete pRoot;
	pRoot = new Node(pTemp->key, pTemp->pLeft, pTemp->pRight, pTemp->freq, pTemp->lvl);
	pTemp->key = pA->key;
	pTemp->freq = pA->freq;
	pTemp->lvl = pA->lvl;
	if (pA->pLeft) {
		pTemp->pLeft = new Node(pA->pLeft->key, pA->pLeft->pLeft, pA->pLeft->pRight, pA->pLeft->freq, pA->pLeft->lvl);
	}
	if (pA->pRight) {
		pTemp->pRight = new Node(pA->pRight->key, pA->pRight->pLeft, pA->pRight->pRight, pA->pRight->freq, pA->pRight->lvl);
	}
	delete pA;
	return 1;
}

int addNewNode(Node *&pRoot, Node *pParent, int temp, char c) {
	if (!pRoot) {
		return 0;
	}
	if (pRoot->key == (char)255) {
		Node *pTemp{ NULL };
		pTemp = pRoot;
		pTemp->key = c;
		pTemp->freq += 1;
		createNode(pTemp->pLeft, 255, 0, pRoot->lvl + 1);
		createNode(pTemp->pRight, c,  pRoot->freq ,pRoot->lvl + 1);
		return 1;
	}
	temp = addNewNode(pRoot->pLeft, pParent, temp, c);
	if (temp == 1) {
		return balanceNode(pRoot,pParent);
	}
	temp = addNewNode(pRoot->pRight, pParent, temp, c);
	if (temp == 1) {
		return balanceNode(pRoot,pParent);
	}
}

int addExistedNode(Node *&pRoot, Node *&pParent, int temp, char c) {
	if (!pRoot) {
		return 0;
	}
	if (pRoot->key == c && isLeaf(pRoot)) {
		return balanceNode(pRoot, pParent);
	}
	temp = addExistedNode(pRoot->pLeft, pParent, temp, c);
	if (temp == 1) {
		return balanceNode(pRoot, pParent);
	}
	temp = addExistedNode(pRoot->pRight, pParent, temp, c);
	if (temp == 1) {
		return balanceNode(pRoot, pParent);
	}
}

int reBalance(Node *&pRoot, Node *&pParent, int temp, char c) { //cân bằng nhánh cây vừa swap node
	if (!pRoot) {
		return 0;
	}
	if (pRoot->key == c && isLeaf(pRoot)) {
		return 1;
	}
	temp = reBalance(pRoot->pLeft, pParent, temp, c);
	if (temp == 1) {
		return balanceNode(pRoot, pParent);
	}
	temp = reBalance(pRoot->pRight, pParent, temp, c);
	if (temp == 1) {
		return balanceNode(pRoot, pParent);
	}
}

int updateTree(Node *&pRoot, char c) { //cân bằng lại cây sau khi thêm node
	bool check{ false };
	search(pRoot, check, c);
	if (!check) {
		addNewNode(pRoot, pRoot, 0, c);
		reBalance(pRoot, pRoot, 0, c);
		return 0;
	}
	else {
		addExistedNode(pRoot, pRoot, 0, c);
		reBalance(pRoot, pRoot, 0, c);
		return 1;
	}
}

void deleteNode(Node *&pRoot) {
	delete pRoot;
	pRoot = NULL;
	return;
}

void deleteTree(Node *&pRoot) {
	if (!pRoot) {
		return;
	}
	deleteTree(pRoot->pLeft);
	if (pRoot && isLeaf(pRoot)) {
		return deleteNode(pRoot);
	}
	deleteTree(pRoot->pRight);
	if (pRoot && isLeaf(pRoot)) {
		return deleteNode(pRoot);
	}
}

char findChar(Node *pRoot, const std::string code, int i) { //tìm ký tự trong cây bằng mã nhị phân
	char c{ code[i] };
	if (isLeaf(pRoot)) {
		return pRoot->key;
	}
	if (i == code.size()) {
		return (char)255;
	}
	if (c == '0') {
		return findChar(pRoot->pLeft, code, ++i);
	}
	if (c == '1') {
		return findChar(pRoot->pRight, code, ++i);
	}
}

void compressFile() { 
	std::string fileInput;
	std::cout << "File Input Address: ";
	std::getline(std::cin, fileInput);
	std::cout << std::endl;

	std::string addressFileOutput;
	std::cout << "File Output Address: ";
	std::getline(std::cin, addressFileOutput);
	std::string data{ readDataFromFile(fileInput) };

	Node *pRoot{ NULL };
	initiateTree(pRoot);
	for (int i{ 0 }; i < data.size(); i++) {
		int check;
		check = adaptiveHuffmanEncoding(addressFileOutput, pRoot, data[i]);
		updateTree(pRoot, data[i]);
	}
	deleteTree(pRoot);
	return;
}

int extractFile() {
	std::string fileInput;
	std::cout << "File Input Address: ";
	std::getline(std::cin, fileInput);
	std::cout << std::endl;

	std::string addressFileOutput;
	std::cout << "File Output Address: ";
	std::getline(std::cin, addressFileOutput);
	std::string data{ readDataFromFile(fileInput) };

	Node *pRoot{ NULL };
	initiateTree(pRoot);
	std::string temp{ "" };
	int mode{ 1 };
	int j{ 0 };
	for (int i{ 0 }; i < data.size(); i++) { //check lần lượt từng mã bit một, nếu == với mà đến nút escape thì xét tiếp 8 bit tiếp theo, không thì xét lại từng node 
		if (i != 0) {
			temp += data[i];
			if (i == data.size() - 1) {
				mode = 3;
			}
			mode = adaptiveHuffmanDecoding(addressFileOutput, pRoot, temp, mode, j);
		}
		else {
			i = -1;
		}
		if (mode == 0) {
			continue;
		}
		if (mode == 1) {
			temp = "";
			++i;
			while (true) {
				temp += data[i];
				if (temp.size() == 8) {
					break;
				}
				++i;
			}
			char cTemp{ convertBinaryToDecimal(temp) };
			std::string sTemp{ cTemp };
			writeDataToFile(addressFileOutput, sTemp, 1);
			updateTree(pRoot, cTemp);
			temp = "";
			continue;
		}
		if (mode == 2) {
			i -= j;
			temp = "";
			j = 0;
			char cTemp;
			std::string sTemp;
			while (true) {
				temp += data[i];
				cTemp = findChar(pRoot, temp, j);
				sTemp = cTemp;
				if (cTemp != (char)255) {
					break;
				}
				++i;
			}
			writeDataToFile(addressFileOutput, sTemp, 1);
			updateTree(pRoot, cTemp);
			temp = "";
			continue;
		}
	}
	return 1;
}

int main() {
	//compressFile();
	extractFile();
	return 0;
}