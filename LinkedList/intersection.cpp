#include <bits/stdc++.h>
using namespace std;

struct Node
{
    int data;
    Node* next;
    Node(int v)
    {
        data = v;
        next = nullptr;
    }
};

void detectintersection(Node* head1, Node* head2)
{
    Node* temp1 = head1;
    

    while(temp1 != nullptr)
    {
        Node* temp2 = head2;
        while(temp2 != nullptr)
        {
            if(temp1 == temp2)
            {
                cout << "Intersection at node with data: " << temp1->data << endl;
                return;
            }
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }
    cout << "No intersection detected." << endl;
}

void detectintersectionUsingHashing(Node* head1, Node* head2)
{
    unordered_set<Node*> visited;
    Node* temp = head1;
    while(temp != nullptr)
    {
        visited.insert(temp);
        temp = temp->next;
    }
    
    temp = head2;
    while(temp != nullptr)
    {
        if(visited.find(temp) != visited.end())
        {
            cout << "Intersection at node with data: " << temp->data << endl;
            return;
        }
        temp = temp->next;
    }
    cout << "No intersection detected." << endl;
}



int main()
{
    
    // Create first linked list with 100 nodes
    Node* head1 = new Node(1);
    Node* temp1 = head1;
    for (int i = 2; i <= 10000; i++) {
        temp1->next = new Node(i);
        temp1 = temp1->next;
    }
    Node* intersectionNode = temp1; // Last node (10000) will be intersection point
    
    // Create second linked list with 5000 nodes, then point to last node of first list
    Node* head2 = new Node(10001);
    Node* temp2 = head2;
    for (int i = 10002; i <= 15000; i++) {
        temp2->next = new Node(i);
        temp2 = temp2->next;
    }
    temp2->next = intersectionNode; // Intersection at node with data 10000
    
    // Display first list
    cout << "List 1: ";
    Node* temp = head1;
    while (temp) {
        cout << temp->data << " -> ";
        temp = temp->next;
    }
    cout << "NULL\n";
    
    // Display second list  
    cout << "List 2: ";
    temp = head2;
    while (temp) {
        cout << temp->data << " -> ";
        temp = temp->next;
    }
    cout << "NULL\n";
    
    
    detectintersection(head1, head2);
    auto start = chrono::high_resolution_clock::now();
    detectintersection(head1, head2);
    auto end = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Time taken by detectintersection: " << duration1.count() << " microseconds\n";
    
    start = chrono::high_resolution_clock::now();
    detectintersectionUsingHashing(head1, head2);
    end = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Time taken by detectintersectionUsingHashing: " << duration2.count() << " microseconds\n";
    
    return 0;
}