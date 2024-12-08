#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>

//#define DEBUG

using namespace std;

const int INF = 100000000;

int n, WIDTH;

struct Rectangle {
	int width;
	int height;

	Rectangle() :width(0), height(0) {}

	Rectangle(int w, int h) :width(w), height(h) {}
};

struct space {
	int x; // Start with 0
	int y; // Start with 0
	int width;
	int h1; // Left height
	int h2; // Right height

	space(int x, int y, int w, int h1, int h2) :x(x), y(y), width(w), h1(h1), h2(h2) {}
};

// For testing
void PrintSpace(vector<space> spaces)
{
	for (size_t i = 0; i < spaces.size(); ++i) {
		const space& s = spaces[i];
		cout << "Space " << i << ": "
			<< "x = " << s.x << ", "
			<< "y = " << s.y << ", "
			<< "width = " << s.width << ", "
			<< "h1 = " << s.h1 << ", "
			<< "h2 = " << s.h2 << endl;
	}
}

// Find the lowest and the most left space s
int findBestSpace(vector<space>& spaces)
{
	int min_y = INF;
	int min_Index = -1;
	for (int i = 0; i < (int)spaces.size(); i++) {
		if (spaces[i].y < min_y) {
			min_y = spaces[i].y;
			min_Index = i;
		}
	}
	return min_Index;
}

// Calculate Fitness
int calculateFitness(Rectangle& i, int h1, int h2, int w)
{
	if (h1 >= h2) {
		if (w == i.width && h1 == i.height) return 4;
		else if (w == i.width && h1 < i.height) return 3;
		else if (w == i.width && h1 > i.height) return 2;
		else if (w > i.width && h1 == i.height) return 1;
		else if (w > i.width && h1 != i.height) return 0;
		else return -1;
	}
	else {
		// h1 < h2
		if (w == i.width && h2 == i.height) return 4;
		else if (w == i.width && h2 < i.height) return 3;
		else if (w == i.width && h2 > i.height) return 2;
		else if (w > i.width && h2 == i.height) return 1;
		else if (w > i.width && h2 != i.height) return 0;
		else return -1;
	}
}

// Make Spaces Flat Function
void MakeSpacesFlat(vector<space>& spaces, int s)
{
	if (spaces.size() == 1) {
		throw runtime_error("Unexpected Manipulation happened");
	}

	// At begin
	if (s == 0) {
		spaces[1].h1 = INF;
		spaces[1].width += spaces[0].width;
		spaces[1].x = 0;
		spaces.erase(spaces.begin());
	}
	// At end
	else if (s == spaces.size() - 1) {
		spaces[s - 1].width += spaces[s].width;
		spaces[s - 1].h2 = INF;
		spaces.erase(spaces.begin() + s);
	}
	// At middle
	else {
		// Left and Right are at same height
		if (spaces[s].h1==spaces[s].h2) {
			spaces[s - 1].width = spaces[s - 1].width + spaces[s].width + spaces[s + 1].width;
			spaces[s - 1].h2 = spaces[s + 1].h2;
			spaces.erase(spaces.begin() + s);
			spaces.erase(spaces.begin() + s);
		}
		// Left is higher than Right
		else if (spaces[s].h1 > spaces[s].h2) {
			spaces[s].y = spaces[s + 1].y;
			spaces[s].width += spaces[s + 1].width;
			spaces[s].h1 -= spaces[s].h2;
			spaces[s].h2 = spaces[s + 1].h2;
			spaces.erase(spaces.begin() + s + 1);
		}
		// Right is higher than Left
		else {
			spaces[s - 1].width += spaces[s].width;
			spaces[s - 1].h2 = spaces[s].h2 - spaces[s].h1;
			spaces.erase(spaces.begin() + s);
		}
	}
}

// Pack while space.size == 1
void PackIntoEmptySpace(vector<space>& spaces, Rectangle& R, int fitness)
{
	if (fitness == 4) {
		throw runtime_error("Unexpected manipulation happened");
	}
	else if (fitness == 3) {
		throw runtime_error("Unexpected manipulation happened");
	}
	else if (fitness == 2) {
		spaces[0].y += R.height;
	}
	else if (fitness == 1) {
		throw runtime_error("Unexpected manipulation happened");
	}
	else if (fitness == 0) {
		spaces[0].y += R.height;
		spaces[0].width = R.width;
		spaces[0].h2 = -1;
		spaces.push_back(space(R.width, spaces[0].y-R.height, WIDTH - R.width, R.height, INF));
	}
	else {
		throw runtime_error("Unexpected manipulation happened");
	}
}

// Pack rectangle R near to the most left place
void PackToLeft_Begin(vector<space>& spaces, Rectangle& R, int fitness)
{
	if (fitness == 4) {
		throw runtime_error("Unexpected manipulation happened");
	}
	else if (fitness == 3) {
		throw runtime_error("Unexpected manipulation happened");
	}
	else if (fitness == 2) {
		// Same as "MakeSpacesFlat"
		if (spaces[0].h2 == R.height) {
			spaces[1].x = 0;
			spaces[1].width += spaces[0].width;
			spaces[1].h1 = INF;
			spaces.erase(spaces.begin());
		}
		// Still lower than Right
		else if (spaces[0].h2 > R.height) {
			spaces[0].y += R.height;
			spaces[0].h2 -= R.height;
		}
		// Higher than Right
		else {
			spaces[1].h1 = R.height - spaces[0].h2;
			spaces[0].y += R.height;
			spaces[0].h2 = -1;
		}
	}
	else if (fitness == 1) {
		throw runtime_error("Unexpected manipulation happened");
	}
	else if (fitness == 0) {
		// Insert to the most left, spaces[0] represents new spaces[1]
		spaces[0].x = R.width;
		spaces[0].width -= R.width;
		spaces[0].h1 = R.height;
		space newSpace0(0, spaces[0].y + R.height, R.width, INF, -1);
		spaces.insert(spaces.begin(), newSpace0);
	}
	else {
		throw runtime_error("Unexpected manipulation happened");
	}
}

// Pack rectangle R near to the most right place
void PackToRight_End(int s, vector<space>& spaces, Rectangle& R, int fitness)
{
	if (fitness == 4) {
		throw runtime_error("Unexpected manipulation happened");
	}
	else if (fitness == 3) {
		throw runtime_error("Unexpected manipulation happened");
	}
	else if (fitness == 2) {
		// Same as "MakeSpacesFlat"
		if (spaces[s].h1 == R.height) {
			spaces[s - 1].width += spaces[s].width;
			spaces[s - 1].h2 = INF;
			spaces.erase(spaces.begin() + s);
		}
		// Higher than Left
		else if (R.height > spaces[s].h1) {
			spaces[s - 1].h2 = R.height - spaces[s].h1;
			spaces[s].y += R.height;
			spaces[s].h1 = -1;
		}
		// Still lower than left
		else {
			spaces[s].y += R.height;
			spaces[s].h1 -= R.height;
		}
	}
	else if (fitness == 1) {
		throw runtime_error("Unexpected manipulation happened");
	}
	else if (fitness == 0) {
		// Insert to the most right
		spaces[s].width -= R.width;
		spaces[s].h2 = R.height;
		space newRightSpace(WIDTH - R.width, spaces[s].y + R.height, R.width, -1, INF);
		spaces.push_back(newRightSpace);
	}
	else {
		throw runtime_error("Unexpected manipulation happened");
	}
}

// Pack while Left and Right are at same height
void PackToLeft_SameHeight(int s, vector<space>& spaces, Rectangle& R, int fitness)
{
	// Same as "MakeSpaces Flat", erasing two spaces
	if (fitness == 4) {
		spaces[s - 1].width = spaces[s - 1].width + spaces[s].width + spaces[s + 1].width;
		spaces[s - 1].h2 = spaces[s + 1].h2;
		spaces.erase(spaces.begin() + s);
		spaces.erase(spaces.begin() + s);
	}
	// Higher than Left and Right
	else if (fitness == 3) {
		spaces[s - 1].h2 = R.height - spaces[s].h1;
		spaces[s + 1].h1 = R.height - spaces[s].h2;
		spaces[s].y += R.height;
		spaces[s].h1 = -1;
		spaces[s].h2 = -1;
	}
	// Lower than Left and Right
	else if (fitness == 2) {
		spaces[s].y += R.height;
		spaces[s].h1 -= R.height;
		spaces[s].h2 -= R.height;
	}
	// Left.width up, this.width down
	else if (fitness == 1) {
		spaces[s - 1].width += R.width;
		spaces[s].x += R.width;
		spaces[s].width -= R.width;
	}
	// Insert a new space
	else if (fitness == 0) {
		// spaces[s] represents the new spaces[s+1]
		// Still lower than Left
		if (spaces[s].h1 > R.height) {
			space newSpace_s(spaces[s].x, spaces[s].y + R.height, R.width, spaces[s].h1 - R.height, -1);
			spaces[s].x += R.width;
			spaces[s].width -= R.width;
			spaces[s].h1 = R.height;
			spaces.insert(spaces.begin() + s, newSpace_s);
		}
		// Higher than Left
		else {
			spaces[s - 1].h2 = R.height - spaces[s].h1;
			space newSpace_s(spaces[s].x, spaces[s].y + R.height, R.width, -1, -1);
			spaces[s].x += R.width;
			spaces[s].width -= R.width;
			spaces[s].h1 = R.height;
			spaces.insert(spaces.begin() + s, newSpace_s);
		}
	}
	else {
		throw runtime_error("Unexpected manipulation happened");
	}
}

// Pack to left while Left is higher than Right
void PackToLeft_LeftIsHigher(int s, vector<space>& spaces, Rectangle& R, int fitness)
{
	// Erase one space
	if (fitness == 4) {
		spaces[s - 1].width += R.width;
		spaces[s + 1].h1 = spaces[s].h1 - spaces[s].h2;
		spaces.erase(spaces.begin() + s);
	}
	// space[s] becomes higher than Left and Right
	else if (fitness == 3) {
		spaces[s - 1].h2 = R.height - spaces[s].h1;
		spaces[s + 1].h1 = R.height - spaces[s].h2;
		spaces[s].y += R.height;
		spaces[s].h1 = -1;
		spaces[s].h2 = -1;
	}
	// space[s] is lower than Left
	else if (fitness == 2) {
		// Same as "MakeSpacesFlat", erase one space
		if (R.height == spaces[s].h2) {
			spaces[s].width += spaces[s + 1].width;
			spaces[s].h1 -= R.height;
			spaces[s].h2 = spaces[s + 1].h2;
			spaces[s].y += R.height;
			spaces.erase(spaces.begin() + s + 1);
		}
		// space[s] is higher than Right
		else if (R.height > spaces[s].h2) {
			spaces[s + 1].h1 = R.height - spaces[s].h2;
			spaces[s].y += R.height;
			spaces[s].h1 -= R.height;
			spaces[s].h2 = -1;
		}
		// space[s] is lower than Right
		else {
			spaces[s].y += R.height;
			spaces[s].h1 -= R.height;
			spaces[s].h2 -= R.height;
		}
	}
	// Left.width up, this.width down, same as PackToLeft_SameHeight
	else if (fitness == 1) {
		spaces[s - 1].width += R.width;
		spaces[s].width -= R.width;
		spaces[s].x += R.width;
	}
	// Insert a newspace, same as PackToLeft_SameHeight
	else if (fitness == 0) {
		// spaces[s] represents new spaces[s+1]
		// Still lower than Left
		if (R.height < spaces[s].h1) {
			space newSpace_s(spaces[s].x, spaces[s].y + R.height, R.width, spaces[s].h1 - R.height, -1);
			spaces[s].x += R.width;
			spaces[s].width -= R.width;
			spaces[s].h1 = R.height;
			spaces.insert(spaces.begin() + s, newSpace_s);
		}
		// Higher than Left
		else {
			space newSpace_s(spaces[s].x, spaces[s].y + R.height, R.width, -1, -1);
			spaces[s - 1].h2 = R.height - spaces[s].h1;
			spaces[s].h1 = R.height;
			spaces[s].x += R.width;
			spaces[s].width -= R.width;
			spaces.insert(spaces.begin() + s, newSpace_s);
		}
	}
}

// Pack to right while Right is higher than Left
void PackToRight_RightIsHigher(int s, vector<space>& spaces, Rectangle& R, int fitness)
{
	// Erase one space
	if (fitness == 4) {
		spaces[s - 1].h2 = spaces[s].h2 - spaces[s].h1;
		spaces[s + 1].x = spaces[s].x;
		spaces[s + 1].width += R.width;
		spaces.erase(spaces.begin() + s);
	}
	// space[s] is higher than Right
	else if (fitness == 3) {
		spaces[s + 1].h1 = R.height - spaces[s].h2;
		spaces[s - 1].h2 = R.height - spaces[s].h1;
		spaces[s].y += R.height;
		spaces[s].h1 = -1;
		spaces[s].h2 = -1;
	}
	// space[s] is lower than Right
	else if (fitness == 2) {
		// Same height as Left, erase one space
		if (spaces[s].h1 == R.height) {
			spaces[s - 1].width += R.width;
			spaces[s - 1].h2 = spaces[s].h2 - spaces[s].h1;
			spaces.erase(spaces.begin() + s);
		}
		// Lower than Left
		else if (spaces[s].h1 > R.height) {
			spaces[s].y += R.height;
			spaces[s].h1 -= R.height;
			spaces[s].h2 -= R.height;
		}
		// Higher than Left
		else {
			spaces[s - 1].h2 = R.height - spaces[s].h1;
			spaces[s].y += R.height;
			spaces[s].h1 = -1;
			spaces[s].h2 -= R.height;
		}
	}
	// Right.width up, this.width down
	else if (fitness == 1) {
		spaces[s].width -= R.width;
		spaces[s + 1].x -= R.width;
		spaces[s + 1].width += R.width;
	}
	// Insert a new space
	else if (fitness == 0) {
		// Higher than Right
		// spaces[s] represents new spaces[s+1]
		if (spaces[s].h2 < R.height) {
			space newSpace_s(spaces[s].x, spaces[s].y, spaces[s].width - R.width, spaces[s].h1, R.height);
			spaces[s + 1].h1 = R.height - spaces[s].h2;
			spaces[s].x += (spaces[s].width - R.width);
			spaces[s].y += R.height;
			spaces[s].width = R.width;
			spaces[s].h1 = -1;
			spaces[s].h2 = -1;
			spaces.insert(spaces.begin() + s, newSpace_s);
		}
		// Lower than Right
		else {
			space newSpace_s(spaces[s].x, spaces[s].y, spaces[s].width - R.width, spaces[s].h1, R.height);
			spaces[s].x += (spaces[s].width - R.width);
			spaces[s].y += R.height;
			spaces[s].width = R.width;
			spaces[s].h1 = -1;
			spaces[s].h2 -= R.height;
			spaces.insert(spaces.begin() + s, newSpace_s);
		}
	}
}

// Pack rectangle R near to the left wall
void PackToLeft(int s, vector<space>& spaces, Rectangle& R, int fitness)
{
	// There is only one space
	if (spaces.size() == 1) {
		if (s == 0) {
#ifdef DEBUG
			cout << "PackIntoEmptySpace" << endl;
#endif
			PackIntoEmptySpace(spaces, R, fitness);
		}
		else {
			throw runtime_error("Unexpected manipulation happened");
		}
	}
	// At end, which is unexpected
	else if (s == (int)spaces.size() - 1) {
		throw runtime_error("Unexpected manipulation happened");
	}
	// At begin
	else if (s == 0) {
#ifdef DEBUG
		cout << "PackToLeft_Begin" << endl;
#endif
		PackToLeft_Begin(spaces, R, fitness);
	}
	// Left and Right are at same height
	else if (spaces[s].h1 == spaces[s].h2) {
#ifdef DEBUG
		cout << "PackToLeft_SameHeight" << endl;
#endif
		PackToLeft_SameHeight(s, spaces, R, fitness);
	}
	// Left is higher than Right
	else {
#ifdef DEBUG
		cout << "PackToLeft_LeftIsHigher" << endl;
#endif
		PackToLeft_LeftIsHigher(s, spaces, R, fitness);
	}
}

// Pack rectangle R near to the right wall
void PackToRight(int s, vector<space>& spaces, Rectangle& R, int fitness)
{
	if (spaces.size() == 1) {
		cout << "Unexpected manipulation happened, there might be some bugs in program" << endl;
		spaces[0].h1 = INF;
		spaces[0].h2 = INF;
		PackToLeft(s, spaces, R, fitness);
	}
	// At begin, which is unexpected
	else if (s == 0) {
		throw runtime_error("Unexpected manipulation happened");
	}
	// At end
	else if (s == (int)spaces.size() - 1) {
#ifdef DEBUG
		cout << "PackToRight_End" << endl;
#endif
		PackToRight_End(s, spaces, R, fitness);
	}
	else {
#ifdef DEBUG
		cout << "PackToRight_RightIsHigher" << endl;
#endif
		PackToRight_RightIsHigher(s, spaces, R, fitness);
	}
}

// SkyLine main function
int SkyLine(vector<Rectangle>& Rectangles)
{
	int h = 0;
	int pin = 0;

	vector<space> spaces;
	spaces.push_back(space(0, 0, WIDTH, INF, INF));

	while (pin < n) {
#ifdef DEBUG
		cout << "Step " << pin << " Finished!" << endl;
		cout << "Result >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl;
		PrintSpace(spaces);
		cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< " << endl;
		cout << endl;
#endif
		int s = findBestSpace(spaces);
#ifdef DEBUG
		cout << "Selected : " << s << endl;
#endif

		if (spaces[s].width >= Rectangles[0].width) {
			// Select one Rec R with the maximum score
			int highestFitness = -1;
			int highestFitnessIndex = -1;
			for (int i = 0; i < (int)Rectangles.size(); i++) {
				int fitness = calculateFitness(Rectangles[i], spaces[s].h1, spaces[s].h2, spaces[s].width);
				if (fitness > highestFitness) {
					highestFitness = fitness;
					highestFitnessIndex = i;
				}
				if (fitness == 4) {
					break;
				}
			}
#ifdef DEBUG
			cout << "Fitness : " << highestFitness << endl;
#endif
			Rectangle R = Rectangles[highestFitnessIndex];
			Rectangles.erase(Rectangles.begin() + highestFitnessIndex);

#ifdef DEBUG
			cout << "This Rectangle : " << endl;
			cout << "Width = " << R.width << " ; Height = " << R.height << endl;
#endif
			pin++;

			// Update h
			if (spaces[s].y + R.height > h) {
				h = spaces[s].y + R.height;
			}

			if (spaces[s].h1 >= spaces[s].h2) {
#ifdef DEBUG
				cout << "PackToLeft" << endl;
#endif
				PackToLeft(s, spaces, R, highestFitness);
			}
			else {
#ifdef DEBUG
				cout << "PackToRight" << endl;
#endif
				PackToRight(s, spaces, R, highestFitness);
			}
		}
		else {
#ifdef DEBUG
			cout << "WARNING : THIS STEP IS MakeSpaces Flat" << endl;
			cout << "Chosen space : " << s << endl;
#endif
			MakeSpacesFlat(spaces, s);
		}
	}

	return h;
}

int main()
{
	cin >> n >> WIDTH;

	vector<Rectangle> Rectangles(n);

	for (int i = 0; i < n; i++) {
		cin >> Rectangles[i].width;
		if (Rectangles[i].width > WIDTH) {
			throw runtime_error("Rectangle width out of range error");
		}
	}
	for (int i = 0; i < n; i++) {
		cin >> Rectangles[i].height;
	}

	// Increasing order
	sort(Rectangles.begin(), Rectangles.end(), [](const Rectangle& a, const Rectangle& b) {
		return a.width < b.width; 
		});

	int result = SkyLine(Rectangles);
	cout << result << endl;

	return 0;
}
