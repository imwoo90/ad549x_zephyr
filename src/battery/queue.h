#pragma once

#include <stdbool.h>

#define MAX_SIZE 20  // 큐의 최대 크기

// 큐가 비어있는지 확인
bool isEmpty();
// 큐가 가득 찼는지 확인
bool isFull();
// 큐에 데이터 추가
void enqueue(int data);
// 큐에서 데이터 제거 및 반환
int dequeue();
// 큐의 앞부분 데이터 확인
int peek();
int getItemCount();