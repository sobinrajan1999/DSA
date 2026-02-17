#include <bits/stdc++.h>
using namespace std;

struct Node
{
    int data;
    Node *next;
    Node(int x)
    {
        data = x;
        next = nullptr;
    }
};

void detectCycle(Node* head)
{
    Node* slow = head;
    Node* fast = head;

    while(fast != nullptr and fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next->next;
        if(slow == fast)
        {
            cout<<"Cycle detected at node with value: " << slow->data << endl;
            return;
        }
    }
    cout<<"No cycle detected." << endl;
}

void straightforwardDetectCycle(Node* head)
{
    unordered_set<Node*> visited;
    Node* temp = head;
    while(temp!= nullptr)
    {
        if(visited.find(temp) != visited.end())
        {
            cout<<"Cycle detected at node with value: " << temp->data << endl;
            return;
        }
        visited.insert(temp);
        temp = temp->next;
    }
    cout<<"No cycle detected." << endl;
}

void removeCycle(Node* head)
{
    Node* slow = head;
    Node* fast = head;

    // Detect cycle
    while(fast != nullptr and fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next->next;
        if(slow == fast)
        {
            break;
        }
    }

    // If cycle exists, find the start of the cycle
    if(slow == fast)
    {
        slow = head;
        while(slow->next != fast->next)
        {
            slow = slow->next;
            fast = fast->next;
        }
        // Remove cycle
        fast->next = nullptr;
        cout<<"Cycle removed. Cycle started at node with value: " << slow->data << endl;
    }
}

void print(Node* head)
{
    Node* temp = head;
    while(temp != nullptr)
    {
        cout << temp->data << " ";
        temp = temp->next;
    }
    cout << endl;
}





int main()
{
    // Create nodes
    Node *head = new Node(1);
    Node *second = new Node(2);
    Node *third = new Node(3);
    Node *fourth = new Node(4);

    // Link nodes
    head->next = second;
    second->next = third;
    third->next = fourth;
    fourth->next = second;  // Creates cycle: 2 -> 3 -> 4 -> 2

    detectCycle(head);
    straightforwardDetectCycle(head);
    removeCycle(head);
    print(head);

    return 0;
}