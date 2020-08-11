#ifndef TEST_HW_03_IOBITITERATOR_HPP
#define TEST_HW_03_IOBITITERATOR_HPP

#include <iterator>

struct input_bit_iterator final {
    //typedef bool value_type;
    //typedef std::input_iterator_tag iterator_category;

    input_bit_iterator();

    explicit input_bit_iterator(std::istreambuf_iterator<char> &istream_iterator);

    bool operator*() const;

    void operator++();

    bool operator!=(const input_bit_iterator &other) const;

    std::size_t get_count() const;

    void flush();

private:
    std::istreambuf_iterator<char> istream_iterator_;
    unsigned char char_now_ = 0, pos_ = 0;
    std::size_t byte_count_ = 0;
};

struct out_bit_iterator final {
    //typedef bool value_type;
    //typedef std::output_iterator_tag iterator_category;

    explicit out_bit_iterator(std::ostreambuf_iterator<char> &ostream_iterator);

    out_bit_iterator &operator++();

    bool &operator*();

    out_bit_iterator &operator=(bool push_bit);

    void flush();

    std::size_t get_count() const;

private:
    char get_char();

    std::ostreambuf_iterator<char> &ostream_iterator_;
    bool char_now_[8];
    std::size_t byte_count_ = 0, pos_ = 0;
};


#endif //TEST_HW_03_IOBITITERATOR_HPP
