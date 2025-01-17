
#include "linked_list.h"

extern Node* playing_song;

DoubleLinkedList::DoubleLinkedList()
{
    head = nullptr;
}

void DoubleLinkedList::addSong(Song& song)
{

    if(doesExist(song.song_name , song.artist) && song.artist != "Unknown")
    {
        return;
    }



    Node* newSong = new Node(song);

    if (head == nullptr)
    {
        head = newSong;
        return;
    }


    
    Node* current = head;
    
    while(current->next != nullptr)
    {
        current = current->next;
    }
    current->next = newSong;
    newSong->pre = current;
    

}
// will chack if the --New-- music music is already exists in our playlists or not
bool DoubleLinkedList::doesExist(std::string song_name, std::string artist)
{
    std::cout << "oi\n";
    if (head == nullptr)
    {
        return false;
    }

    Node* current = head;

    while (current != nullptr)
    {
        if (current->song.artist == artist && current->song.song_name == song_name)
        {
            return true;
        }
        current = current->next;
    }
    std::cout << "oi\n";
    
    return false;
}

// remove the music from the playlist by artist (if exists)
void DoubleLinkedList::removeSong_by_artist(std::string artist)
{

    if (head == nullptr)
    {
        return;
    }
    

    Node* current = head;

    while (current != nullptr)
    {
        if (current->song.artist == artist)
        {
            Node* deletedNode = current;

            if (deletedNode == head)
            {
                head = head->next;
                if (head != nullptr)
                {
                    head->pre = nullptr;
                }
                
            }
            else
            {
                if (deletedNode->pre)
                {
                    deletedNode->pre->next = deletedNode->next;
                }

                if (deletedNode->next)
                {
                    deletedNode->next->pre = deletedNode->pre;
                }
                
            }

            current = current->next;
            delete deletedNode;
        }

        current = current->next;
        
    }
    
}

// remove the music from the playlist by music title (if exists)
void DoubleLinkedList::removeSonge_by_name(std::string song_name)
{
    if (head == nullptr)
    {
        return;
    }

    Node* current = head;

    while (current != nullptr)
    {

        if (current->song.song_name == song_name)
        {

            if (current == head)
            {
                head = head->next;
                if (head != nullptr)
                {
                    head->pre = nullptr;
                }
                
            }
            else
            {
                if (current->pre)
                {
                    current->pre->next = current->next;
                }

                if (current->next)
                {
                    current->next->pre = current->pre;
                }
                
            }

            delete current;
            return;
            
        }
        
        
        current = current->next;
    }
    
}

// same as removeSong_by_artist function , but in the artist search 
void DoubleLinkedList::removeSong_by_artist_genre_search(std::string artist , Genre genre)
{
    if (head == nullptr)
    {
        return;
    }

    Node* current = head;

    while (current != nullptr)
    {

        std::string artist_song = UpperCase(current->song.artist);
        std::string input_artist_song = UpperCase(artist);

        if (!input_artist_song.empty() && artist_song.find(input_artist_song) != std::string::npos)
        {

            if (current->song.genre != genre && genre != All)
            {
                current = current->next;
                continue;
            }
            

            Node* deletedNode = current;


            if (deletedNode == head)
            {
                head = head->next;
                if (head != nullptr)
                {
                    head->pre = nullptr;
                }
            }
            else
            {

                if (deletedNode->pre)
                {
                    deletedNode->pre->next = deletedNode->next;
                }

                if (deletedNode->next)
                {
                    deletedNode->next->pre = deletedNode->pre;
                }
            }

            current = current->next;
            delete deletedNode;
        }
        else
        {
            current = current->next;
        }
    }
}

// sort the playlists base on our filter (MUSIC_TITLE or MUSIC_ARTIST)
void DoubleLinkedList::Sort(SORT_FILTER base_on)
{

    if (head == nullptr)
    {
        return;
    }

    // selection sort algorithm , and preventing unexpected and weird stuff
    for (Node* i = head; i != nullptr; i = i->next)
    {
        Node* minimum = i; 

        for (Node* j = i->next; j != nullptr; j = j->next)
        {
            std::string temp1 = (base_on == MUSIC_TITLE)? UpperCase(minimum->song.song_name) : UpperCase(minimum->song.artist);
            
            std::string temp2 = (base_on == MUSIC_TITLE)? UpperCase(j->song.song_name) : UpperCase(j->song.artist); 
            
            if (temp1 > temp2)
            {
                minimum = j;
            }
            
        }

        if (minimum != i)
        {
            Song temp = i->song;
            i->song = minimum->song;
            minimum->song = temp;

            if (playing_song == nullptr)    
                continue;
            
            if (playing_song == minimum)
            {
                playing_song = i;
            }
            else if(playing_song == i)
            { 
                playing_song = minimum;
            }
            
            
        }
        
    }

}