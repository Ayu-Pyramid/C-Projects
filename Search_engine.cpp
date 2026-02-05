#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>

using namespace std;

/* =========================
   Document
   ========================= */
class Document {
private:
    int documentId;
    string fileName;
    string content;

public:
    Document(int d = 0, string f = "", string c = "") {
        documentId = d;
        fileName = f;
        content = c;
    }

    int getDocumentId() const { return documentId; }
    string getFileName() const { return fileName; }
    string getContent() const { return content; }

    string toFileString() const {
        return to_string(documentId) + "|" + fileName + "|" + content;
    }

    static Document fromFileString(const string& line) {
        int p1 = line.find('|');
        int p2 = line.find('|', p1 + 1);

        int id = stoi(line.substr(0, p1));
        string file = line.substr(p1 + 1, p2 - p1 - 1);
        string cont = line.substr(p2 + 1);

        return Document(id, file, cont);
    }
};

/* =========================
   Index Entry
   ========================= */
struct IndexEntry {
    string word;
    int documentId;
    int count;
};

/* =========================
   Helpers
   ========================= */
string cleanWord(string w) {
    string result;
    for (char c : w) {
        if (isalpha(c))
            result += tolower(c);
    }
    return result;
}

void indexDocument(const Document& doc, vector<IndexEntry>& index) {
    stringstream ss(doc.getContent());
    string word;

    while (ss >> word) {
        word = cleanWord(word);
        if (word.empty()) continue;

        bool found = false;
        for (auto& e : index) {
            if (e.word == word && e.documentId == doc.getDocumentId()) {
                e.count++;
                found = true;
                break;
            }
        }

        if (!found) {
            index.push_back({word, doc.getDocumentId(), 1});
        }
    }
}

/* =========================
   Search Engine
   ========================= */
class SearchEngine {
private:
    vector<Document> documents;
    vector<IndexEntry> index;

public:
    void addDocument(const Document& d) {
        documents.push_back(d);
    }

    void buildIndex() {
        index.clear();
        for (const auto& d : documents) {
            indexDocument(d, index);
        }
    }

    void search(const string& query) {
        string q = cleanWord(query);
        cout << "\nResults for '" << q << "':\n";

        bool found = false;
        for (const auto& e : index) {
            if (e.word == q) {
                cout << "Document " << e.documentId
                     << " | score " << e.count << endl;
                found = true;
            }
        }

        if (!found)
            cout << "No results found.\n";
    }
};

/* =========================
   Main
   ========================= */
int main() {
    SearchEngine engine;

    engine.addDocument(
        Document(1, "doc1.txt",
        "Hello world hello security")
    );

    engine.addDocument(
        Document(2, "doc2.txt",
        "Security is important in the modern world")
    );

    engine.buildIndex();

    engine.search("hello");
    engine.search("security");
    engine.search("missing");

    return 0;
}
