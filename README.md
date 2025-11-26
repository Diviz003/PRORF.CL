# PRORF.CL
---

# prorf.cl

*A C++ Command-Line Tool for ORF & Promoter Site Detection in DNA/RNA Sequences*

---

## 📌 Overview

**prorf.cl** is a lightweight, high-performance **Open Reading Frame (ORF) & promoter-detection CLI tool** written in C++.
Built for offline bioinformatics workflows, it analyzes raw DNA/RNA sequence data directly within the terminal — without web servers, without data upload, and with full local privacy.

The tool identifies:

🔹 TATA-box (Eukaryotic promoter)
🔹 Pribnow-box (Prokaryotic promoter)
🔹 ORFs in DNA and RNA (Start → Stop codon based detection)

Designed for speed, CLI integration, and educational insight, **prorf.cl** demonstrates core gene prediction logic similar to ORFFinder, GenScan and ORFik — but implemented in a simple, clean C++ architecture.

---

## 🧠 Motivation

Accurate ORF detection is fundamental to:

* gene expression analysis
* protein translation prediction
* viral genome annotation
* transcriptomic gene discovery

Most ORF tools require the internet or heavy computational frameworks.
**prorf.cl removes both barriers — fast, offline, pure C++.**

---

## ✨ Features

| Feature                                      | Supported |
| -------------------------------------------- | :-------: |
| ORF detection in DNA (ATG → TAA/TAG/TGA)     |     ✔     |
| ORF detection in RNA (AUG → UAA/UAG/UGA)     |     ✔     |
| Promoter detection (Eukaryotic TATA-box)     |     ✔     |
| Promoter detection (Prokaryotic Pribnow-box) |     ✔     |
| Command-line usage                           |     ✔     |
| Offline & local data processing              |     ✔     |
| FASTA-friendly multiline input parsing       |     ✔     |

---

## ⚙️ Architecture

```
┌──────────────────────┐
│ User Input (FASTA)   │
└─────────┬────────────┘
          ▼
📄 Preprocessing → Uppercase | Noise removal
          ▼
🔍 Promoter Finder (TATA / TATAAT)
          ▼
🧬 ORF Detector (Start→Stop codon scan)
          ▼
📤 Results Summary to Terminal
```

Built using modular components:

| Module           | Responsibility                          |
| ---------------- | --------------------------------------- |
| `SequenceReader` | Reads & sanitizes raw sequence input    |
| `PatternMatcher` | Detects promoter motif occurrences      |
| `ORFDetector`    | Locates ORFs & extracts coding segments |

---

## 🔧 Core Libraries

| Library       | Use                                   |
| ------------- | ------------------------------------- |
| `<iostream>`  | Terminal input/output                 |
| `<string>`    | Sequence storage & manipulation       |
| `<vector>`    | Dynamic result storage                |
| `<sstream>`   | Multiline FASTA assembly              |
| `<algorithm>` | Case normalization + pattern matching |
| `<limits>`    | Clean input buffer handling           |

### Custom Structure

```cpp
struct OpenReadingFrame {
    int startIndex;
    int stopIndex;
    std::string sequence;
};
```

---

## 🔍 How It Works

### Promoter Scan

* Eukaryotic → finds **TATAAA**
* Prokaryotic → finds **TATAAT**
* RNA mode skips promoter search

Sliding-window algorithm scans the entire sequence and records promoter indices.

### ORF Detection Logic

1. Scan nucleotides for start codon
2. When found → scan forward in *triplets*
3. Stop scanning at first valid termination codon
4. Store ORF sequence + start & stop indices

---

## ▶️ Usage

```bash
g++ prorf.cpp -o prorf
./prorf
```

Follow interactive CLI prompts:

```
Select sequence type:
1) Eukaryotic DNA
2) Prokaryotic DNA
3) RNA
Paste your sequence below:
(Press CTRL+D when finished)
```

Output example format:

```
Detected Promoters:
 - TATA Box at index: 134

Detected ORFs:
#1 Start: 120 | Stop: 454 | Length: 334bp
Sequence: ATGCG...TAA
```

---

## 🚀 Future Development

Planned enhancements include:

🔸 Reverse-strand ORF scanning
🔸 GC-content & codon-bias analysis
🔸 Machine-learning promoter/ORF prediction
🔸 GPU/multithread acceleration for large genomes
🔸 Export as JSON/CSV + visualization modules

---

## 📜 License

Open and free for academic & research use.

