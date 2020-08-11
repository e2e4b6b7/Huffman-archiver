#include "IOBitIterator.hpp"


input_bit_iterator::input_bit_iterator() : istream_iterator_{std::istreambuf_iterator<char>{}} {}

input_bit_iterator::input_bit_iterator(std::istreambuf_iterator<char> &istream_iterator) : istream_iterator_{istream_iterator} {
    char_now_ = *istream_iterator_;
}

bool input_bit_iterator::operator*() const {
    return char_now_ & (1u << (7u - pos_));
}

void input_bit_iterator::operator++() {
    if (pos_ == 7) {
        ++istream_iterator_;
        ++byte_count_;
        pos_ = 0;
        char_now_ = *istream_iterator_;
    } else {
        ++pos_;
    }
}

bool input_bit_iterator::operator!=(const input_bit_iterator &other) const {
    return istream_iterator_ != other.istream_iterator_;
}

std::size_t input_bit_iterator::get_count() const {
    return byte_count_;
}

void input_bit_iterator::flush() {
    if (pos_ != 0) {
        ++istream_iterator_;
        ++byte_count_;
        char_now_ = *istream_iterator_;
        pos_ = 0;
    }
}


out_bit_iterator::out_bit_iterator(std::ostreambuf_iterator<char> &ostream_iterator) : ostream_iterator_{ostream_iterator} {}


out_bit_iterator &out_bit_iterator::operator++() {
    ++pos_;
    if (pos_ == 8) {
        *ostream_iterator_ = get_char();
        ++byte_count_;
        pos_ = 0;
    }
    return *this;
}

bool &out_bit_iterator::operator*() {
    return char_now_[pos_];
}

void out_bit_iterator::flush() {
    if (pos_ != 0) {
        *ostream_iterator_ = get_char();
        ++byte_count_;
        pos_ = 0;
    }
}

std::size_t out_bit_iterator::get_count() const {
    return byte_count_;
}

char out_bit_iterator::get_char() {
    unsigned char c{0};
    for (std::size_t i = 0; i < pos_; ++i) {
        if (char_now_[i]) c |= (1u << (7 - i));
    }
    return static_cast<char>(c);
}

out_bit_iterator &out_bit_iterator::operator=(bool b) {
    char_now_[pos_] = b;
    operator++();
    return *this;
}
