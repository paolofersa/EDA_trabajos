#include <iostream>
#include <vector>
#include <tuple>
#include <cfloat>
#include <climits>		//INT_MAX, INT_MIN
#include <algorithm>	//sort

using namespace std;

struct RNode
{
	bool leaf;										//is it a leaf?
	vector <tuple<float, float>> I;				//store (do,df) for n dimensions	BOUNDING BOX
	RNode* child_ptr;								//dynamic array of RNode's (size M)
	vector <tuple<float, float>> tuple_id;		//store the object

	void addValuesI(float a, float b) {
		I.push_back(make_tuple(a, b));
	}

	RNode() {
		leaf = true;
		child_ptr = nullptr;
	}

};

class RTree
{
public:
	RTree(int _M);
	~RTree();
	RNode* root;

	bool insert(vector <tuple<float, float>> E);
	void search(vector <tuple<float, float>> S, RNode**& T);

private:
	int M;		//max number of entries
	int m;		//min number of entries

	bool chooseLeaf(vector <tuple<float, float>> E, RNode**& N);
	float findEnlargement(vector <tuple<float, float>> boundingBox, vector <tuple<float, float>> entry);
	void linearSplitNode(RNode**& L, RNode*& LL, vector <tuple<float, float>> entry);
	void adjustTree(RNode**& L);
	bool overlaps(RNode*& node, vector <tuple<float, float>> entry);
};

RTree::RTree(int _M)
{
	root = 0;
	M = _M;
	m = _M / 2;
}

RTree::~RTree()
{
}

void RTree::search(vector <tuple<float, float>> S, RNode**& T) {
	//first value of T = &root
	//S1
	if ((*T) && !(*T)->leaf) {			//is not a leaf
		//check each entry E if overlap S
		for (RNode* E = (*T); E->I.size() != 0 && E < (*T) + M; E++) {
			if (overlaps(E, S)) {
				RNode** Ep = &(E->child_ptr);
				search(S, Ep);
			}
		}
	}
	else{								//is a leaf
		for (RNode* E = (*T); E->I.size() != 0 && E < (*T) + M; E++) {
			if (overlaps(E, S)) {
				vector<tuple<float, float>> qualifyingRecord = E->tuple_id;
				//is a qualifying record
				cout << "El registro de coordenadas\t";
				for (int i = 0; i < qualifyingRecord.size(); i++) {
					cout << "d" << i;
					cout << " [" << get<0>(qualifyingRecord[i]) << ", " << get<1>(qualifyingRecord[i]) << "]\t";
				}
				cout << "coincide con la entrada dada\n";
			}
		}
	}
}

bool RTree::insert(vector <tuple<float, float>> E) {
	RNode** L;
	chooseLeaf(E, L);		//I1
	//I2
	if (((*L) + M - 1)->tuple_id.size() == 0) {		//room for another entry?
		bool cond = true;
		for (RNode* it = (*L); cond; it++) {		//search the entry with no value
			if (it->tuple_id.size() != 0) {
				it->tuple_id = E;
				it->I = E;
				cond = false;
			}
		}
	}
	else	{
		RNode* LL;
		linearSplitNode(L, LL, E);
		adjustTree(L);
	}
}

bool RTree::chooseLeaf(vector <tuple<float, float>> E, RNode**& N) {
	RNode* Fs;
	for (N = &root; *N && !((*N)->leaf); N = &(Fs->child_ptr)) {	//CL1, CL2
		//CL3
		//Find de least enlargement
		vector<RNode*> choosenFs;							//store F's in N with least enlargement
		choosenFs.push_back(*N);
		float leastEnlargment = findEnlargement((*N)->I, E);

		for (RNode* F = (*N) + 1; F->I.size() != 0 && F < (*N) + M; F++) {		//all entries in N with values
			float enlargementF = findEnlargement(F->I, E);
			if (leastEnlargment > enlargementF) {
				leastEnlargment = enlargementF;
				choosenFs.clear();
			}
			if (leastEnlargment == enlargementF)
				choosenFs.push_back(F);
		}
		//in case of ties
		if (choosenFs.size() > 1) {
			//set first area as the min
			float minArea = 1;
			for (vector<tuple<float, float>>::iterator it = choosenFs[0]->I.begin(); it != choosenFs[0]->I.end(); it++)
				minArea = minArea * (get<1>(*it) - get<0>(*it));
			//compare area with other nodes
			for (vector<RNode*>::iterator itNodes = choosenFs.begin() + 1; itNodes != choosenFs.end(); itNodes++) {
				float areaItNodes = 1;
				for (vector<tuple<float, float>>::iterator it = (*itNodes)->I.begin(); it != (*itNodes)->I.end(); it++)
					areaItNodes = areaItNodes * (get<1>(*it) - get<0>(*it));
				if (minArea > areaItNodes) {
					minArea = areaItNodes;
					choosenFs[0] = *itNodes;
				}

			}

		}
		//Get de entry od the node
		Fs = choosenFs[0];
	}
	return *N != 0;
}

void RTree::linearSplitNode(RNode**& L, RNode*& LL, vector <tuple<float, float>> entry) {

}

void RTree::adjustTree(RNode**& L) {

}

float RTree::findEnlargement(vector <tuple<float, float>> boundingBox, vector <tuple<float, float>> entry) {
	float actualArea = 1;			//area of bounding box
	for (vector<tuple<float, float>>::iterator it = boundingBox.begin(); it != boundingBox.end(); it++)
		actualArea = actualArea * (get<1>(*it) - get<0>(*it));

	float biggestArea = 1;				//area that encloses both rectangles
	vector<tuple<float, float>> minMaxValues;
	float min, max;

	vector<tuple<float, float>>::iterator it1 = boundingBox.begin();
	vector<tuple<float, float>>::iterator it2 = entry.begin();
	for (int i = 0; i < boundingBox.size(); i++) {
		vector<float> list;					//will store sorted points
		list.push_back(get<0>(*it1));
		list.push_back(get<1>(*it1));
		list.push_back(get<0>(*it2));
		list.push_back(get<1>(*it2));
		sort(list.begin(), list.end());		//sort points
		min = *list.begin();
		max = *list.end();
		minMaxValues.push_back(make_tuple(min, max));	//get minMax values
		it1++;
		it2++;
	}

	for (vector<tuple<float, float>>::iterator it = minMaxValues.begin(); it != minMaxValues.end(); it++)
		biggestArea = biggestArea * (get<1>(*it) - get<0>(*it));

	return biggestArea - actualArea;
}

bool  RTree::overlaps(RNode*& node, vector <tuple<float, float>> entry) {
	for (int i = 0; i < node->I.size(); i++) {
		if (get<0>(entry[i]) >= get<0>(node->I[i]) &&
			get<0>(entry[i]) <= get<1>(node->I[i]))
			return true;
		if (get<1>(entry[i]) >= get<0>(node->I[i]) &&
			get<1>(entry[i]) <= get<1>(node->I[i]))
			return true;
	}
	return false;
}

//CODIGO PRINCIPAL
int main(){
    
	//INSERTION
	float x1, x2, y1, y2;
	vector <tuple<float, float>> entry;
	entry.push_back(make_tuple(x1,x2));
	entry.push_back(make_tuple(y1, y2));
	
    return 0;
}