#include "ofApp.h"
#include <string.h>
#include <queue>
#include <deque>
#include <iostream>
using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Rotating Maze");
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);

	// flag�� 0���� ���� �ʱ�ȭ��Ų��
	open_flag = 0;
	draw_flag = 0;	
	solve_flag = 0;
	dfs_flag = 0;
	maze_index = 0;

	/* �� ���α׷��� ������ �ܼ�â�� ���� ���� */
	cout << " ******************* HOW TO RUN *******************" << endl;
	cout << " 1. press 'L' or 'l' to load maze file (~40*40 size)" << endl;
	cout << " 2. press 'D' or 'd' to draw maze " << endl;
	cout << " 3. press 'S' or 's' to solve the maze " << endl;
	cout << " 4. press 'Q' or 'q' to end this program " << endl;
	cout << " 5. press 'R' or 'r' to rotate the maze " << endl;
	cout << " **************************************************\n" << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
	// dfs �ƴ� daikstra �˰����� ��� �Ǵ� ������ ȭ�鿡 ���������� ��Ÿ���� ���� �� �����Ӹ��� s�� �� �� ��ǥ�� pop�Ѵ�
	if (dfs_flag) {
		if (s.empty()) dfs_flag = 0;
		else s.pop_back();
	}

	if (solve_flag) {
		if (s.empty()) solve_flag = 0;
		else s.pop_back();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0, 0, 0, 0);
	ofSetColor(100);

	int i, j;
	if (draw_flag && open_flag) {
		// ȭ�鿡 �̷θ� �׸���
		// ������ ��ȸ������ �׷��ش�
		for (i = 1;i < WIDTH + 1;i++) {
			for (j = 1;j < WIDTH + 1;j++) {
				if (!input[i][j]) ofDrawRectangle(j * 20, i * 20, 20, 20);
			}
		}

		// �̷��� �׵θ� ������ �׷��ش�
		for (i = 0;i <= WIDTH + 1;i++) {
			ofDrawRectangle(0, i * 20, 20, 20);
			ofDrawRectangle((WIDTH + 1) * 20, i * 20, 20, 20);
			if (i == 0 || i == WIDTH + 1) {
				for (j = 1;j <= WIDTH;j++) ofDrawRectangle(j * 20, i * 20, 20, 20);
			}
		}

		// �̷��� ������� ǥ���Ѵ�
		ofSetColor(192, 201, 187);
		if (maze_index == 0) ofDrawRectangle(20, 20, 20, 20);
		else if(maze_index == 1) ofDrawRectangle(WIDTH * 20, 20, 20, 20);
		else if (maze_index == 2) ofDrawRectangle(WIDTH * 20, WIDTH * 20, 20, 20);
		else if (maze_index == 3) ofDrawRectangle(20, WIDTH * 20, 20, 20);

		// �Ķ��� ������ dfs �˰������� ã�� �ش��� ���������� ǥ���Ѵ�
		if (dfs_flag && !s.empty()) {
			ofSetColor(0, 0, 255);
			ofDrawCircle(s.back().second * 20 + 10, s.back().first * 20 + 10, 10);
		}

		// �Ķ��� ������ daikstra �˰����� Ž�� ������ ���������� ǥ���Ѵ�
		if (solve_flag && !s.empty()) {
			ofSetColor(0, 0, 255);
			ofDrawCircle(s.back().second * 20 + 10, s.back().first * 20 + 10, 10);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	// ������ �ҷ��´�
	if (key == 'l' || key == 'L') {
		open_flag = 0;
		draw_flag = 0;
		solve_flag = 0;
		dfs_flag = 0;
		maze_index = 0;

		processOpenFileSelecton();
	}

	// �ҷ��� ������ �������� ȭ�鿡 �̷θ� �׷��ش�
	if (key == 'd') draw_flag = 1;
	
	// ���α׷��� �����Ѵ�
	if (key == 'q' || key == 'Q') {
		open_flag = 0;
		draw_flag = 0;
		solve_flag = 0;
		dfs_flag = 0;

		// input �迭�� �Ҵ�� �޸𸮸� �����Ѵ�
		delete[] input;
		ofExit();
	}

	// �̸� �ҷ��Դ� �̷θ� ���������� 90�� ȸ����Ų��
	if (key == 'r' || key == 'R') {
		draw_flag = 0;
		solve_flag = 0;
		dfs_flag = 0;

		maze_index++;
		if (maze_index == 4) maze_index = 0;

		rotate();
		draw_flag = 1;
	}

	// �̷��� �ذ�å�� ã�Ƴ���
	if (key == 's' || key == 'S') {
		solve();
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

bool ofApp::processOpenFileSelecton() {
	/* ������ �˸°� �ҷ��´� */
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;

	// ������ ������ ������ ���
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");
		string fileName = openFileResult.getName();
		printf("file name is %s\n", fileName.c_str()); // ���� ������ �̸��� ����Ѵ�
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		
		// ���� �̸��� ���� Ȯ���ڰ� maz���� �˻��Ѵ�
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			// Ȯ���ڰ� maz�� �ƴ϶�� ���� �ε��� �ߴ��Ѵ�
			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
			}

			ofBuffer buffer(file);

			int cnt = 0; // �̷� �迭�� ���̸� ��Ÿ���� ����
			input = new int*[41];
			input[0] = new int[41];

			// ������ ������ �� �� �� �޾Ƽ� input �迭�� �˸°� �����Ѵ�
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
				string line = *it;
				WIDTH = line.size();
				if (WIDTH > 40 || cnt > 40) return false; // �̷��� ũ�Ⱑ 40 * 40�� ����� ���� �ҷ����� �� �����Ѵ�
				cnt++;
				input[cnt] = new int[WIDTH];
				for (int j = 0;j < WIDTH;j++) input[cnt][j + 1] = (int)line[j] - 48; // �迭�� ������ 1 ~ WIDTH�� �ǰ� �Ѵ�
			}

			// �̷��� �ʺ�� ���̰� ������ �˻��Ѵ�
			if (cnt != WIDTH) {
				cout << "Please use a maze that has same width & height" << endl;
				return false;
			}
			open_flag = 1; // �˸°� ������ ������, ���嵵 �� �Ǿ����Ƿ� open_flag�� 1�� �����Ѵ�
		}
		// Ȯ���ڰ� maz�� �ƴ� ���
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
	return true;
}

void ofApp::rotate() {
	int rotation[41][41]; // ȸ���� �迭�� ������ �ӽ� �����ϴ� �迭
	// ���������� 90�� ȸ����Ų input �迭�� ������ rotation�� �ӽ÷� �����Ѵ�
	for (int i = 1;i <= WIDTH;i++) {
		for (int j = 1;j <= WIDTH;j++) {
			rotation[j][WIDTH + 1 - i] = input[i][j];
		}
	}

	// rotation �迭�� ������ �״�� input �迭�� ������ ���������� input �迭�� ���������� 90�� ȸ���ϰ� �Ǿ���
	for (int i = 1;i <= WIDTH;i++) {
		for (int j = 1;j <= WIDTH;j++) {
			input[i][j] = rotation[i][j];
		}
	}
}

void ofApp::solve() {
	// �̷θ� ȭ�鿡 �׸��� (d) �� �������� �ⱸ�� �����ϴ� �ذ�å�� ã�� �� �ִ�
	if (!draw_flag) {
		cout << "Please draw the maze first" << endl;
		return;
	}
	
	s.clear();
	q.clear();

	/* DFS algorithm�� ����� �ⱸ�� �����ϴ� �ذ�å�� ã�´� */
	int visited[1681]; // 41 * 41 ũ���, �̷ο��� �ش� ��ġ�� �湮�ߴ��� �˷��ش�
	memset(visited, 0, sizeof(visited)); // visited �迭�� 0���� �ʱ�ȭ�Ѵ�

	int startx, starty, exitx, exity; // maze_index�� ���� �޶����� �����, �������� ��ǥ�� �����ϴ� ������
	if (maze_index == 0) {
		startx = 1;
		starty = 1;
		exitx = WIDTH;
		exity = WIDTH;
	}
	else if (maze_index == 1) {
		startx = 1;
		starty = WIDTH;
		exitx = WIDTH;
		exity = 1;
	}
	else if (maze_index == 2) {
		startx = WIDTH;
		starty = WIDTH;
		exitx = 1;
		exity = 1;
	}
	else if (maze_index == 3) {
		startx = WIDTH;
		starty = 1;
		exitx = 1;
		exity = WIDTH;
	}
	else return;

	// �ʱ� ������ ���ش�
	s.push_front({ startx, starty }); // s�� ���� ��ǥ�� push �Ѵ�
	visited[convert(startx, starty)] = 1; // ���� ��ǥ�� Ž���ߴٰ� ǥ���Ѵ�

	while (!s.empty()) {
		// ���� ��ǥ�� �������̸� Ž���� �����Ѵ�
		if (s.front().first == exitx && s.front().second == exity) {
			dfs_flag = 1; // �ⱸ������ ���� ã�����Ƿ� dfs_flag = 1�� �����Ѵ�
			return;
		}

		// s�� ù��° ��ǥ�� ������ ���� �����Ѵ�
		int currx = s.front().first;
		int curry = s.front().second;
		int k = 0;
		// dx, dy �迭�� �̿��� ���� ��ǥ�� ��, ��, ��, �츦 �˻��Ͽ� 
		// ���� �迭�� ���� ���� ��ġ�̰�, �̵��� �� �ְ�, ���� Ž���� ���� ���ٸ� s�� �ش� ��ǥ�� push �ϰ� Ž���ߴٰ� ǥ�����ش�
		for (; k < 4; k++) {
			int nextx = currx + dx[k];
			int nexty = curry + dy[k];
			if (!limit(nextx, nexty) && input[nextx][nexty] && !visited[convert(nextx, nexty)]) {
				s.push_front({ nextx, nexty });
				visited[convert(nextx, nexty)] = 1;
				break;
			}
		}
		// ���� ��ġ���� �� �̻� �̵��� �� ������ ���� ��ǥ�� pop �Ѵ�
		if (k == 4) {
			s.pop_front();
		}
	}

	/* ���� ���� ã�� �� ���ٸ�(dfs_flag = 0), ������ �� �ְ� �մ� �ּ����� ���� ������ Dijkstra algorithm�� �̿��� ���Ѵ� */
	int dis[1681]; // ��������κ��� ���� ��ġ���� �����ϱ� ���� �վ�� �� ���� ������ �����ϴ� �迭
	memset(dis, 0x3f, sizeof(dis)); // dis �迭�� 0x3f�� �ʱ�ȭ�Ѵ�
	s.clear(); // �� dfs ���� ����ߴ� s�� �ٽ� ����ϱ� ������ ����ش�
	dis[convert(startx, starty)] = 0; // ó�� ��ġ�� �ش��ϴ� dis ���� 0���� �ʱ�ȭ�Ѵ�
	q.push_back({ startx, starty }); // ó�� ��ǥ�� q�� �� �ڿ� �����Ѵ�

	while (!q.empty()) {
		// q�� �� ���� ��ǥ, �� ���� ��ǥ�� ���� ������ �������ش�
		int currx = q.front().first;
		int curry = q.front().second;
		int curr = convert(currx, curry);
		q.pop_front(); // q�� �� ���� ��ǥ�� pop �Ͽ� �����Ѵ�

		for (int k = 0; k < 4; k++) {
			// ���� ��ġ���� ��, ��, ��, �츦 ��Ÿ���� ��ǥ�� dx, dy �迭�� ���� �����Ѵ�
			int nextx = currx + dx[k];
			int nexty = curry + dy[k];
			int next = convert(nextx, nexty);

			if (limit(nextx, nexty)) continue; // ���� ���� ��ǥ�� ���� �ٱ��� ��ǥ��� q�� �������� �ʴ´�
			int cost = input[nextx][nexty] ? 0 : 1; // ���� ��ǥ�� ������ ������ ��Ÿ���� ����
			// ���� ��ǥ�� dis ���� ���� ��ǥ�� dis �� + cost �� ���� ���ڰ� �� ũ�� ���ڷ� ��ü�Ѵ�
			if (dis[next] > dis[curr] + cost) {
				dis[next] = dis[curr] + cost;
				if (cost == 1) q.push_back({ nextx, nexty });
				else q.push_front({ nextx, nexty });
				s.push_front({ nextx, nexty }); // Ž�� ������ �����Ѵ�
			}
		}
	}
	solve_flag = 1; // �ּ����� ���� ������ �������Ƿ� solve_flag�� 1�� �����Ѵ�

	// �ּ����� ���� ������ �ܼ�â�� ����Ѵ�
	cout << "Num of walls you should break: ";
	cout << dis[convert(exitx, exity)] << endl;
}

int ofApp::convert(int i, int j) {
	return (i - 1) * WIDTH + j;
}

bool ofApp::limit(int i, int j) {
	return (i > WIDTH || i <= 0 || j > WIDTH || j <= 0);
}