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

	// flag를 0으로 전부 초기화시킨다
	open_flag = 0;
	draw_flag = 0;	
	solve_flag = 0;
	dfs_flag = 0;
	maze_index = 0;

	/* 이 프로그램의 사용법을 콘솔창에 먼저 띄운다 */
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
	// dfs 아님 daikstra 알고리즘의 결과 또는 과정을 화면에 순차적으로 나타내기 위해 매 프레임마다 s의 맨 뒤 좌표를 pop한다
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
		// 화면에 미로를 그린다
		// 벽면을 연회색으로 그려준다
		for (i = 1;i < WIDTH + 1;i++) {
			for (j = 1;j < WIDTH + 1;j++) {
				if (!input[i][j]) ofDrawRectangle(j * 20, i * 20, 20, 20);
			}
		}

		// 미로의 테두리 벽면을 그려준다
		for (i = 0;i <= WIDTH + 1;i++) {
			ofDrawRectangle(0, i * 20, 20, 20);
			ofDrawRectangle((WIDTH + 1) * 20, i * 20, 20, 20);
			if (i == 0 || i == WIDTH + 1) {
				for (j = 1;j <= WIDTH;j++) ofDrawRectangle(j * 20, i * 20, 20, 20);
			}
		}

		// 미로의 출발점을 표시한다
		ofSetColor(192, 201, 187);
		if (maze_index == 0) ofDrawRectangle(20, 20, 20, 20);
		else if(maze_index == 1) ofDrawRectangle(WIDTH * 20, 20, 20, 20);
		else if (maze_index == 2) ofDrawRectangle(WIDTH * 20, WIDTH * 20, 20, 20);
		else if (maze_index == 3) ofDrawRectangle(20, WIDTH * 20, 20, 20);

		// 파란색 원으로 dfs 알고리즘으로 찾은 해답을 순차적으로 표시한다
		if (dfs_flag && !s.empty()) {
			ofSetColor(0, 0, 255);
			ofDrawCircle(s.back().second * 20 + 10, s.back().first * 20 + 10, 10);
		}

		// 파란색 원으로 daikstra 알고리즘의 탐색 과정을 순차적으로 표시한다
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
	// 파일을 불러온다
	if (key == 'l' || key == 'L') {
		open_flag = 0;
		draw_flag = 0;
		solve_flag = 0;
		dfs_flag = 0;
		maze_index = 0;

		processOpenFileSelecton();
	}

	// 불러온 파일을 바탕으로 화면에 미로를 그려준다
	if (key == 'd') draw_flag = 1;
	
	// 프로그램을 종료한다
	if (key == 'q' || key == 'Q') {
		open_flag = 0;
		draw_flag = 0;
		solve_flag = 0;
		dfs_flag = 0;

		// input 배열에 할당된 메모리를 해제한다
		delete[] input;
		ofExit();
	}

	// 미리 불러왔던 미로를 오른쪽으로 90도 회전시킨다
	if (key == 'r' || key == 'R') {
		draw_flag = 0;
		solve_flag = 0;
		dfs_flag = 0;

		maze_index++;
		if (maze_index == 4) maze_index = 0;

		rotate();
		draw_flag = 1;
	}

	// 미로의 해결책을 찾아낸다
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
	/* 파일을 알맞게 불러온다 */
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;

	// 파일이 무사히 열렸을 경우
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");
		string fileName = openFileResult.getName();
		printf("file name is %s\n", fileName.c_str()); // 열린 파일의 이름을 출력한다
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		
		// 파일 이름을 비교해 확장자가 maz인지 검사한다
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			// 확장자가 maz가 아니라면 파일 로딩을 중단한다
			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
			}

			ofBuffer buffer(file);

			int cnt = 0; // 미로 배열의 높이를 나타내는 변수
			input = new int*[41];
			input[0] = new int[41];

			// 파일의 내용을 한 줄 씩 받아서 input 배열에 알맞게 저장한다
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
				string line = *it;
				WIDTH = line.size();
				if (WIDTH > 40 || cnt > 40) return false; // 미로의 크기가 40 * 40을 벗어나면 파일 불러오는 걸 종료한다
				cnt++;
				input[cnt] = new int[WIDTH];
				for (int j = 0;j < WIDTH;j++) input[cnt][j + 1] = (int)line[j] - 48; // 배열의 범위는 1 ~ WIDTH가 되게 한다
			}

			// 미로의 너비와 높이가 같은지 검사한다
			if (cnt != WIDTH) {
				cout << "Please use a maze that has same width & height" << endl;
				return false;
			}
			open_flag = 1; // 알맞게 파일을 열었고, 저장도 다 되었으므로 open_flag를 1로 설정한다
		}
		// 확장자가 maz가 아닌 경우
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
	return true;
}

void ofApp::rotate() {
	int rotation[41][41]; // 회전한 배열의 내용을 임시 저장하는 배열
	// 오른쪽으로 90도 회전시킨 input 배열의 내용을 rotation에 임시로 저장한다
	for (int i = 1;i <= WIDTH;i++) {
		for (int j = 1;j <= WIDTH;j++) {
			rotation[j][WIDTH + 1 - i] = input[i][j];
		}
	}

	// rotation 배열의 내용을 그대로 input 배열에 복사해 최종적으로 input 배열이 오른쪽으로 90도 회전하게 되었다
	for (int i = 1;i <= WIDTH;i++) {
		for (int j = 1;j <= WIDTH;j++) {
			input[i][j] = rotation[i][j];
		}
	}
}

void ofApp::solve() {
	// 미로를 화면에 그리고 (d) 난 다음에야 출구로 도착하는 해결책을 찾을 수 있다
	if (!draw_flag) {
		cout << "Please draw the maze first" << endl;
		return;
	}
	
	s.clear();
	q.clear();

	/* DFS algorithm을 사용해 출구로 도착하는 해결책을 찾는다 */
	int visited[1681]; // 41 * 41 크기로, 미로에서 해당 위치를 방문했는지 알려준다
	memset(visited, 0, sizeof(visited)); // visited 배열을 0으로 초기화한다

	int startx, starty, exitx, exity; // maze_index에 따라 달라지는 출발점, 도착점의 좌표를 저장하는 변수들
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

	// 초기 설정을 해준다
	s.push_front({ startx, starty }); // s에 시작 좌표를 push 한다
	visited[convert(startx, starty)] = 1; // 시작 좌표를 탐색했다고 표시한다

	while (!s.empty()) {
		// 현재 좌표가 도착점이면 탐색을 종료한다
		if (s.front().first == exitx && s.front().second == exity) {
			dfs_flag = 1; // 출구까지의 길을 찾았으므로 dfs_flag = 1로 설정한다
			return;
		}

		// s의 첫번째 좌표를 변수에 따로 저장한다
		int currx = s.front().first;
		int curry = s.front().second;
		int k = 0;
		// dx, dy 배열을 이용해 현재 좌표의 상, 하, 좌, 우를 검사하여 
		// 만약 배열의 범위 내의 위치이고, 이동할 수 있고, 전에 탐색한 적이 없다면 s에 해당 좌표를 push 하고 탐색했다고 표시해준다
		for (; k < 4; k++) {
			int nextx = currx + dx[k];
			int nexty = curry + dy[k];
			if (!limit(nextx, nexty) && input[nextx][nexty] && !visited[convert(nextx, nexty)]) {
				s.push_front({ nextx, nexty });
				visited[convert(nextx, nexty)] = 1;
				break;
			}
		}
		// 현재 위치에서 더 이상 이동할 수 없으면 현재 좌표를 pop 한다
		if (k == 4) {
			s.pop_front();
		}
	}

	/* 만약 길을 찾을 수 없다면(dfs_flag = 0), 도착할 수 있게 뚫는 최소한의 벽의 개수를 Dijkstra algorithm을 이용해 구한다 */
	int dis[1681]; // 출발점으로부터 현재 위치까지 도착하기 위해 뚫어야 할 벽의 개수를 저장하는 배열
	memset(dis, 0x3f, sizeof(dis)); // dis 배열을 0x3f로 초기화한다
	s.clear(); // 위 dfs 에서 사용했던 s를 다시 사용하기 때문에 비워준다
	dis[convert(startx, starty)] = 0; // 처음 위치에 해당하는 dis 값을 0으로 초기화한다
	q.push_back({ startx, starty }); // 처음 좌표를 q의 맨 뒤에 삽입한다

	while (!q.empty()) {
		// q의 맨 앞의 좌표, 즉 현재 좌표를 따로 변수에 저장해준다
		int currx = q.front().first;
		int curry = q.front().second;
		int curr = convert(currx, curry);
		q.pop_front(); // q의 맨 앞의 좌표를 pop 하여 삭제한다

		for (int k = 0; k < 4; k++) {
			// 현재 위치에서 상, 하, 좌, 우를 나타내는 좌표를 dx, dy 배열을 통해 저장한다
			int nextx = currx + dx[k];
			int nexty = curry + dy[k];
			int next = convert(nextx, nexty);

			if (limit(nextx, nexty)) continue; // 만약 다음 좌표가 범위 바깥의 좌표라면 q에 저장하지 않는다
			int cost = input[nextx][nexty] ? 0 : 1; // 다음 좌표가 벽인지 길인지 나타내는 변수
			// 다음 좌표의 dis 값과 현재 좌표의 dis 값 + cost 를 비교해 전자가 더 크면 후자로 대체한다
			if (dis[next] > dis[curr] + cost) {
				dis[next] = dis[curr] + cost;
				if (cost == 1) q.push_back({ nextx, nexty });
				else q.push_front({ nextx, nexty });
				s.push_front({ nextx, nexty }); // 탐색 과정을 저장한다
			}
		}
	}
	solve_flag = 1; // 최소한의 벽의 개수를 구했으므로 solve_flag를 1로 설정한다

	// 최소한의 벽의 개수를 콘솔창에 출력한다
	cout << "Num of walls you should break: ";
	cout << dis[convert(exitx, exity)] << endl;
}

int ofApp::convert(int i, int j) {
	return (i - 1) * WIDTH + j;
}

bool ofApp::limit(int i, int j) {
	return (i > WIDTH || i <= 0 || j > WIDTH || j <= 0);
}