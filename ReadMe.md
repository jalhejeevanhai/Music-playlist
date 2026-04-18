This Music Playlist Engine is a high-performance backend system built to manage audio playback using industry-standard Data Structures and Algorithms (DSA). 
It replaces basic arrays with specialized structures to handle complex features like seamless looping, instant search, and intelligent queuing.
Core ArchitectureCircular Doubly Linked List: Manages the main playlist, allowing $O(1)$ transitions between tracks and infinite looping from the last song back to the first.
Tree (Prefix Tree): Powers the search engine, providing $O(k)$ autocomplete and title lookups, ensuring search speed remains constant even as the library grows.
Priority Queue: Handles the "Play Next" feature, allowing specific tracks to be injected into the playback sequence ahead of the default order.
Limited Stack: Maintains a "Least Recently Used" style history, allowing users to navigate backward through their listening session while managing memory efficiently.
Key Technical FeaturesState Persistence: Saves and loads the entire playlist state via JSON.
Fisher-Yates Shuffle: Implements a mathematically unbiased randomization of the playlist.
Memory Management: Utilizes smart pointers (in C++) or circular buffers (in Python) to prevent memory leaks and optimize performance.
