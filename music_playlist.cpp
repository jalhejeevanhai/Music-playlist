
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stack>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <iomanip>

// =============================================================================
// SONG MODEL
// =============================================================================
struct Song {
    std::string id;
    std::string title;
    std::string artist;
    int duration; // seconds

    void display() const {
        int mins = duration / 60;
        int secs = duration % 60;
        std::cout << title << " - " << artist << " (" << mins << ":" 
                  << std::setw(2) << std::setfill('0') << secs << ")";
    }
};

// =============================================================================
// CIRCULAR DOUBLY LINKED LIST
// =============================================================================
struct SongNode {
    Song song;
    std::shared_ptr<SongNode> next;
    std::weak_ptr<SongNode> prev; // weak_ptr to prevent circular reference memory leaks

    SongNode(Song s) : song(s) {}
};

class CircularDoublyLinkedList {
private:
    std::shared_ptr<SongNode> head;
    std::shared_ptr<SongNode> tail;
    int size = 0;

public:
    void insertAtEnd(Song s) {
        auto newNode = std::make_shared<SongNode>(s);
        if (!head) {
            head = tail = newNode;
            newNode->next = newNode;
            newNode->prev = newNode;
        } else {
            newNode->prev = tail;
            newNode->next = head;
            tail->next = newNode;
            head->prev = newNode;
            tail = newNode;
        }
        size++;
    }

    std::shared_ptr<SongNode> getHead() { return head; }
    int getSize() { return size; }

    std::shared_ptr<SongNode> findNode(std::string id) {
        if (!head) return nullptr;
        auto curr = head;
        for (int i = 0; i < size; i++) {
            if (curr->song.id == id) return curr;
            curr = curr->next;
        }
        return nullptr;
    }
};

// =============================================================================
// TRIE (Song Search)
// =============================================================================
struct TrieNode {
    std::unordered_map<char, std::shared_ptr<TrieNode>> children;
    std::vector<Song> matchingSongs;
};

class SongTrie {
private:
    std::shared_ptr<TrieNode> root;

    std::string normalize(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

public:
    SongTrie() : root(std::make_shared<TrieNode>()) {}

    void insert(Song s) {
        std::string title = normalize(s.title);
        auto curr = root;
        for (char c : title) {
            if (curr->children.find(c) == curr->children.end()) {
                curr->children[c] = std::make_shared<TrieNode>();
            }
            curr = curr->children[c];
            curr->matchingSongs.push_back(s);
        }
    }

    std::vector<Song> search(std::string prefix) {
        prefix = normalize(prefix);
        auto curr = root;
        for (char c : prefix) {
            if (curr->children.find(c) == curr->children.end()) return {};
            curr = curr->children[c];
        }
        return curr->matchingSongs;
    }
};

// =============================================================================
// PLAYLIST ENGINE
// =============================================================================
class PlaylistEngine {
private:
    CircularDoublyLinkedList playlist;
    std::stack<Song> history;
    std::priority_queue<std::pair<int, std::string>> playNextQueue; // priority, songId
    SongTrie searchIndex;
    std::shared_ptr<SongNode> currentSongNode;

public:
    void addSong(Song s) {
        playlist.insertAtEnd(s);
        searchIndex.insert(s);
        if (!currentSongNode) currentSongNode = playlist.getHead();
    }

    void playNext() {
        if (!currentSongNode) return;

        // Push to history
        history.push(currentSongNode->song);

        // Logic for priority queue would go here (fetch by ID)
        currentSongNode = currentSongNode->next;
        std::cout << "\nNow playing: ";
        currentSongNode->song.display();
        std::cout << std::endl;
    }

    void playPrevious() {
        if (history.empty()) {
            if (currentSongNode) currentSongNode = currentSongNode->prev.lock();
        } else {
            Song prev = history.top();
            history.pop();
            currentSongNode = playlist.findNode(prev.id);
        }
        std::cout << "\nWent back to: ";
        currentSongNode->song.display();
        std::cout << std::endl;
    }

    void search(std::string query) {
        std::vector<Song> results = searchIndex.search(query);
        std::cout << "\nSearch results for '" << query << "':" << std::endl;
        for (const auto& s : results) {
            std::cout << "  - "; s.display(); std::cout << std::endl;
        }
    }
};

// =============================================================================
// MAIN
// =============================================================================
int main() {
    PlaylistEngine engine;

    engine.addSong({"1", "Bohemian Rhapsody", "Queen", 354});
    engine.addSong({"2", "Stairway to Heaven", "Led Zeppelin", 482});
    engine.addSong({"3", "Hotel California", "Eagles", 391});

    std::cout << "Initial song: Bohemian Rhapsody" << std::endl;
    
    engine.playNext();
    engine.playNext();
    engine.playPrevious();

    engine.search("bo");

    return 0;
}