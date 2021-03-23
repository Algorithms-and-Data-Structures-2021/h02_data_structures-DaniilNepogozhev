#include "linked_list.hpp"

#include <cassert>    // assert
#include <stdexcept>  // out_of_range

#include "private/internal.hpp"  // это не тот приват, о котором вы могли подумать

namespace itis {

void LinkedList::Add(Element e) {
  Node* node = new Node(e, nullptr);
  if (size_  == 0){
      assert(head_ == nullptr && tail_ == nullptr);
      head_ = node;
      tail_ = node;
  } else{
      tail_->next = node;
      tail_ = node;
  }
  size_ += 1;
}

void LinkedList::Insert(int index, Element e) {
  internal::check_out_of_range(index, 0, size_ + 1);
    if (size_ == 0 || index == size_){
        Add(e);
    } else if (index == 0){
        Node* new_node = new Node(e, head_);
        head_ = new_node;
        size_ += 1;
    } else{
        Node* current = find_node(index-1);
        Node* new_node = new Node(e, current->next);
        current->next = new_node;
        size_ += 1;
    }

}

void LinkedList::Set(int index, Element e) {
  internal::check_out_of_range(index, 0, size_);
  find_node(index)->data = e;
}

Element LinkedList::Remove(int index) {
  internal::check_out_of_range(index, 0, size_);
    if (index == 0){
        Element temp_val = head_->data;
        Node* temp = head_->next;
        delete head_;
        head_ = temp;
        size_ -= 1;
        return temp_val;
    } else{
        Node* node = find_node(index-1);
        Node* temp = node->next->next;
        Element temp_val = node->next->data;
        delete node->next;
        node->next = temp;
        size_ -= 1;
        return temp_val;
    }
}

void LinkedList::Clear() {
    Node* current = head_;
    Node* next;
    while (current != nullptr){
        next = current->next;
        current->data = Element::UNINITIALIZED;
        current->next = nullptr;
        current = next;
    }
    size_ = 0;
}

Element LinkedList::Get(int index) const {
  internal::check_out_of_range(index, 0, size_);
  return find_node(index)->data;
}

int LinkedList::IndexOf(Element e) const {
    int cnt = 0;
    for (Node* current = head_; current != nullptr; current = current->next){
        if (e == current->data) return cnt;
        cnt += 1;
    }
  return -1;
}

Node *LinkedList::find_node(int index) const {
  assert(index >= 0 && index < size_);
    if (index == size_ - 1) return tail_;
  int cnt = 0;
  for (Node* current = head_; current != nullptr; current = current->next){
      if (cnt == index) return current;
      cnt += 1;
  }
  return nullptr;
}

// РЕАЛИЗОВАНО

LinkedList::~LinkedList() {
  Clear();
}

bool LinkedList::Contains(Element e) const {
  // если индекс не найден, значит и элемента нет
  return IndexOf(e) == kNotFoundElementIndex;
}

int LinkedList::GetSize() const {
  return size_;
}

bool LinkedList::IsEmpty() const {
  return size_ == 0;
}

Element LinkedList::tail() const {
  // вместо выброса ошибки в случае nullptr, римским парламентов было решено возвращать "специальное" значение
  return tail_ ? tail_->data : Element::UNINITIALIZED;
}

Element LinkedList::head() const {
  return head_ ? head_->data : Element::UNINITIALIZED;
}

// === RESTRICTED AREA: необходимо для тестирования ===

LinkedList::LinkedList(const std::vector<Element> &elements) {
  assert(!elements.empty());

  size_ = elements.size();
  head_ = new Node(elements[0], nullptr);

  auto current_node = head_;

  for (int index = 1; index < static_cast<int>(elements.size()); index++) {
    current_node->next = new Node(elements[index], nullptr);
    current_node = current_node->next;
  }
  tail_ = current_node;
}

std::ostream &operator<<(std::ostream &os, const LinkedList &list) {
  if (list.head_ != nullptr && list.tail_ != nullptr) {
    os << "{ ";
    for (auto current_node = list.head_; current_node != list.tail_; current_node = current_node->next) {
      os << internal::elem_to_str(current_node->data) << ", ";
    }
    os << internal::elem_to_str(list.tail_->data) << " }";
  } else {
    os << "{ nullptr }";
  }
  return os;
}

bool operator==(const LinkedList &list, const std::vector<Element> &elements) {
  if (list.size_ != static_cast<int>(elements.size())) return false;
  Node *current_node = list.head_;

  for (const auto e : elements) {
    if (current_node == nullptr) return false;
    if (current_node->data != e) return false;
    current_node = current_node->next;
  }
  return true;
}

}  // namespace itis