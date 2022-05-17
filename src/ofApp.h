#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		// flag
		int draw_flag; // 'd' 누르면 1, 아닐 때는 0
		int open_flag; // 파일이 제대로 열려 미로를 저장하면 1, 아니면 0
		int solve_flag; // daikstra 알고리즘이 제대로 실행되었으면 1, 아니면 0
		int dfs_flag; // dfs 알고리즘이 제대로 실행되었으면 1, 아니면 0

		// struct
		typedef pair<int, int> p; // 배열의 인덱스를 (i, j) 나타내기 위한 pair
		deque<p> s; // dfs 또는 daikstra 알고리즘을 화면에 그리기 위해 저장하는 deque
		deque<p> q; // daikstar 알고리즘에서 배열의 인덱스(좌표)를 저장하는 deque

		int dx[4] = { 1, -1, 0, 0 }; // 현재 위치의 상, 하, 좌, 우를 가리킬 때의 x 좌표
		int dy[4] = { 0, 0, 1, -1 }; // 현재 위치의 상, 하, 좌, 우를 가리킬 때의 y 좌표
		
		int **input; // 미로를 저장하는 배열

		// variable
		int WIDTH, maze_index; // 미로의 너비(높이)와 회전이 어떻게 되어있는지 알려주는 변수

		// function
		bool processOpenFileSelecton(); // 파일을 열어 알맞게 미로를 저장하는 함수
		void rotate(); // 미로를 회전시키는 함수
		void solve(); // 미로의 해결책을 찾는 함수
		int convert(int, int); // solve 함수에서 2차원 배열의 인덱스를 1차원 배열의 인덱스로 변환해주는 함수
		bool limit(int, int); // solve 함수에서 현재 인덱스가 배열의 범위를 벗어나는지 검사하는 함수
};
