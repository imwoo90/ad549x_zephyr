#include "queue.h"

static int queue[MAX_SIZE];  // 데이터를 저장할 배열
static int front = 0;        // 큐의 앞부분을 가리키는 인덱스
static int rear = -1;        // 큐의 뒷부분을 가리키는 인덱스
static int itemCount = 0;    // 큐에 저장된 항목의 수

// 큐가 비어있는지 확인
bool isEmpty() {
    return itemCount == 0;
}

// 큐가 가득 찼는지 확인
bool isFull() {
    return itemCount == MAX_SIZE;
}

int getItemCount() {
    return itemCount;
}

// 큐에 데이터 추가
void enqueue(int data) {
    if (!isFull()) {
        if (rear == MAX_SIZE - 1) {
            rear = -1;
        }
        queue[++rear] = data;  // rear를 증가시키고 데이터 추가
        itemCount++;           // 저장된 항목의 수 증가
    }
}

// 큐에서 데이터 제거 및 반환
int dequeue() {
    if (!isEmpty()) {
        int data = queue[front++];  // front의 데이터를 가져오고 front 증가

        if (front == MAX_SIZE) {
            front = 0;
        }
        itemCount--;  // 저장된 항목의 수 감소
        return data;
    } else {
        return -1;  // 큐가 비어있는 경우 의미 없는 값 반환
    }
}

// 큐의 앞부분 데이터 확인
int peek() {
    return queue[front];
}
