
#pragma once

#include <iostream>
#include "Song.h"

class Node
{
    public:
    Song song;
    Node* next;
    Node* pre;
    Node(Song song) : song(song)
    {
        next = nullptr;
        pre = nullptr;
    }
};

class DoubleLinkedList
{
    public:

    Node* head;

    DoubleLinkedList();
    void addSong(Song& song);
    void removeSong_by_artist(std::string artist);
    void removeSonge_by_name(std::string song_name);
    void removeSong_by_artist_search(std::string artist);
    bool doesExist(std::string song_name , std::string artist); 
    void Sort(SORT_FILTER base_on);
    ~DoubleLinkedList();
};

    //Node* search_by_songName(std::string song_name);
    //Node* search_by_artist(std::string artist);