#include "MetalinkEditor.hpp"
#include "Metalink4Writer.hpp"
#include "Metalink4Reader.hpp"

MetalinkEditor::MetalinkEditor()
{
    selection_ = 0;
}

bool MetalinkEditor::is_empty()
{
    return files_.size() == 0;
}

int MetalinkEditor::num_files()
{
    return files_.size();
}

wxString MetalinkEditor::get_filename(int file)
{
    return files_.at(file).get_filename();
}

void MetalinkEditor::add_file(wxString filename)
{
    add_file(MetalinkFile(filename));
}

void MetalinkEditor::add_file(MetalinkFile file)
{
    files_.push_back(file);
    selection_ = files_.size() - 1;
    update();
}

void MetalinkEditor::add_listener(MetalinkEditorListener* listener)
{
    listeners_.push_back(listener);
}

void MetalinkEditor::select(int file)
{
    if(file < 0 || file >= files_.size()) return;
    selection_ = file;
    update();
}

int MetalinkEditor::get_selection()
{
    return selection_;
}

void MetalinkEditor::remove_file()
{
    if(files_.size() == 0) return;
    // Erase file
    files_.erase(files_.begin() + selection_);
    // Fix selection
    if(selection_ >= files_.size()) selection_ = files_.size() - 1;
    if(selection_ < 0) selection_ = 0;
    // Update
    update();
}

MetalinkFile MetalinkEditor::get_file()
{
    return files_.at(selection_);
}

std::vector<MetalinkFile> MetalinkEditor::get_files()
{
    return files_;
}

void MetalinkEditor::set_file(MetalinkFile& file)
{
    files_.at(selection_) = file;
    update();
}

wxString MetalinkEditor::get_filename()
{
    return filename_;
}

void MetalinkEditor::set_filename(wxString filename)
{
    filename_ = filename;
}

void MetalinkEditor::update()
{
    for(int i = 0; i < listeners_.size(); i++) {
        listeners_.at(i)->update();
    }
}

void MetalinkEditor::save()
{
    if(filename_.empty()) return;
    Metalink4Writer writer(*this);
    writer.save(filename_);
}

void MetalinkEditor::open(wxString filename)
{
    try {
        Metalink4Reader reader(*this);
        bool loaded = reader.load(filename);
        if(!loaded) {
            throw MetalinkLoadError("Unrecognized file format!");
        }
        filename_ = filename;
    } catch(MetalinkLoadError& e) {
        clear();
        throw e;
    }
}

void MetalinkEditor::clear()
{
    files_.clear();
    filename_.clear();
    selection_ = 0;
    update();
}
