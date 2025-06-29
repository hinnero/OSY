//
// Created by Райцев Олег on 29.05.25.
//

#ifndef VISITOR_HPP
#define VISITOR_HPP

class Disk;
class Folder;
class File;
class ImageFile;
class TextFile;

struct Visitor {
    virtual ~Visitor() = default;
    virtual void visit(Disk&) = 0;
    virtual void visit(Folder&) = 0;
    virtual void visit(File&) = 0;
    virtual void visit(ImageFile&) = 0;
    virtual void visit(TextFile&) = 0;
};

#endif // VISITOR_HPP
