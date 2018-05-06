#pragma once

#include <memory>
#include <string>
#include <functional>

// Use the source Luke! Use the source, Luke Treewalker!
// A sample wrapper of the posix fts() tree walking functions.
// The fts() function famliy can do a lot, and this wrapper only
// provides access to some of the features, but it does show how
// fts() can be made more easy to use in an C++ context.
// Adding any missing features are left as an exercise to the reader.
// Other notes:
// - The class uses the Pimpl Idiom (Google it) to hide internal
//   implementation details behind a pointer.


// When TreeWalker runs, it fills out a struct like this for
// each directory or file it encounters.
struct FileInfo
{
  std::string dirname;  // Path to file (relative to start directory)
  std::string basename; // The actual filename
  enum class FType {
    FILE,
    DIR
  } ftype;
};


// When you create a TreeWalker you give it a function with
// this signature:
//     bool somefuncname(const FileInfo&)
// I.e it must take a FileInfo reference and return true or
// false. If your function returns true, the file or directory
// gets returned via the getNextEntry()-method, otherwise it
// is ignored.
using FileSelector = std::function<bool(const FileInfo&)>;


class TreeWalker
{
public:

  // Constructors/destructors
  TreeWalker(const std::string& root,const FileSelector& selector);
  ~TreeWalker();
  
  // A TreeWaker object can't be copy constructed or copy assigned,
  // we don't allow copying at all.
  TreeWalker& operator=(const TreeWalker&) = delete;
  TreeWalker(const TreeWalker&) = delete;
  // But we do allow a TreeWalker object to be moved
  TreeWalker& operator=(const TreeWalker&&) = delete;
  TreeWalker(const TreeWalker&&) = delete;
  
  // Direct (no iterator stuff) interface
  bool getNextEntry(FileInfo& fileinfo);

private:
  struct treewalker_impl;
  std::unique_ptr<treewalker_impl> pimpl_;
};
