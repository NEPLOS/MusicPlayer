
#pragma once

#include <iostream>
#include "Song.h"
// Node class
class Node
{
    public:
    Song song;
    Node* next;
    Node* pre;
    Node(Song song) : song(song)
    {
        next = nullptr; // default values
        pre = nullptr;
    }
};

class DoubleLinkedList
{
    public:

    Node* head;

    DoubleLinkedList();
    bool addSong(Song& song);
    void removeSong_by_artist(std::string artist);
    void removeSonge_by_name(std::string song_name);
    void removeSong_by_artist_genre_search(std::string artist , Genre genre);
    bool doesExist(std::string song_name , std::string artist);
    void shuffle();
    void Sort(SORT_FILTER base_on);
    ~DoubleLinkedList();
};