#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "graphics.h"
#include "spritemap.h"
#include "text.h"
#include "timer.h"

class TitleRoll {
  public:
    TitleRoll() {}

    class Entry {
      public:
        Entry() : text_("text.png"), sprites_("objects.png", 4, 16, 16) {}
        virtual ~Entry() {}

        virtual int height() const = 0;
        virtual void draw(Graphics& graphics, int y) const = 0;
        virtual void update(unsigned int) {}

      protected:
        Text text_;
        SpriteMap sprites_;
    };

    class Title : public Entry {
      public:
        Title(const std::string& text) : title_(text) {}

        int height() const override { return 32; }
        void draw(Graphics& graphics, int y) const override;

      private:
        std::string title_;
    };

    class NamedItem : public Entry {
      public:
        NamedItem(const std::string& name, int sprite1, int sprite2) : timer_(100), name_(name), n1_(sprite1), n2_(sprite2), frame_(n1_) {}
        NamedItem(const std::string& name, int sprite) : NamedItem(name, sprite, sprite) {}

        int height() const override { return 16; }
        void draw(Graphics& graphics, int y) const override;
        void update(unsigned int elapsed) override;

      private:
        Timer timer_;
        std::string name_;
        int n1_, n2_, frame_;
    };

    class Description : public Entry {
      public:
        Description(const std::string& desc);

        int height() const override { return lines_ * 16 + 16; }
        void draw(Graphics& graphics, int y) const override { text_.draw(graphics, desc_, 8, y); }

      private:
        std::string desc_;
        int lines_;
    };

    void update(unsigned int elapsed);
    void draw(Graphics& graphics, int y) const;
    void add_entry(Entry* entry) { entries_.emplace_back(std::move(entry)); }

  private:
    std::vector<std::unique_ptr<Entry>> entries_;
};
