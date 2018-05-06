
#include "treewalker.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <string.h>

using namespace std;


struct TreeWalker::treewalker_impl
{
  treewalker_impl(const string& r,
                  const FileSelector& s)
  : root{r},selector{move(s)}
  {
    char* dirs[2];
    dirs[0] = const_cast<char*>(root.c_str());
    dirs[1] = nullptr;
    pfts = fts64_open(dirs,FTS_NOCHDIR|FTS_LOGICAL,nullptr);
  }
  string root;
  FileSelector selector;
  FTS64* pfts{nullptr};
  FTSENT64* pftsent{nullptr};
};

TreeWalker::TreeWalker(const string& root,
                       const FileSelector& selector)
: pimpl_{make_unique<treewalker_impl>(root,move(selector))}
{
}

TreeWalker::~TreeWalker()
{
  if (pimpl_->pfts != nullptr)
    fts64_close(pimpl_->pfts);
}

bool TreeWalker::getNextEntry(FileInfo& fileinfo)
{
  treewalker_impl& impl = *pimpl_;
  
  if (impl.pfts == nullptr)
    return false;
  
  while ((impl.pftsent = fts64_read(impl.pfts)) != nullptr) {
    if (impl.pftsent->fts_info == FTS_F)
      fileinfo.ftype = FileInfo::FType::FILE;
    else if (impl.pftsent->fts_info == FTS_D)
      fileinfo.ftype = FileInfo::FType::DIR;
    else
      continue;
    string tmp = impl.pftsent->fts_path;
    fileinfo.dirname = tmp.substr(0,tmp.rfind('/'));
    fileinfo.basename = basename(impl.pftsent->fts_path);
    if (impl.selector(fileinfo))
      return true;
  }
  return false;
}
