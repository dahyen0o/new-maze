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
		int draw_flag; // 'd' ������ 1, �ƴ� ���� 0
		int open_flag; // ������ ����� ���� �̷θ� �����ϸ� 1, �ƴϸ� 0
		int solve_flag; // daikstra �˰����� ����� ����Ǿ����� 1, �ƴϸ� 0
		int dfs_flag; // dfs �˰����� ����� ����Ǿ����� 1, �ƴϸ� 0

		// struct
		typedef pair<int, int> p; // �迭�� �ε����� (i, j) ��Ÿ���� ���� pair
		deque<p> s; // dfs �Ǵ� daikstra �˰����� ȭ�鿡 �׸��� ���� �����ϴ� deque
		deque<p> q; // daikstar �˰��򿡼� �迭�� �ε���(��ǥ)�� �����ϴ� deque

		int dx[4] = { 1, -1, 0, 0 }; // ���� ��ġ�� ��, ��, ��, �츦 ����ų ���� x ��ǥ
		int dy[4] = { 0, 0, 1, -1 }; // ���� ��ġ�� ��, ��, ��, �츦 ����ų ���� y ��ǥ
		
		int **input; // �̷θ� �����ϴ� �迭

		// variable
		int WIDTH, maze_index; // �̷��� �ʺ�(����)�� ȸ���� ��� �Ǿ��ִ��� �˷��ִ� ����

		// function
		bool processOpenFileSelecton(); // ������ ���� �˸°� �̷θ� �����ϴ� �Լ�
		void rotate(); // �̷θ� ȸ����Ű�� �Լ�
		void solve(); // �̷��� �ذ�å�� ã�� �Լ�
		int convert(int, int); // solve �Լ����� 2���� �迭�� �ε����� 1���� �迭�� �ε����� ��ȯ���ִ� �Լ�
		bool limit(int, int); // solve �Լ����� ���� �ε����� �迭�� ������ ������� �˻��ϴ� �Լ�
};
