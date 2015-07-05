//
// Created by rhunter on 6/20/15.
//

#ifndef IEEE754_IEEE754_H
#define IEEE754_IEEE754_H

#include <cstdint>
#include <functional>
#include <gtkmm/togglebutton.h>

class Ieee754
{

public:
    Ieee754(const Glib::RefPtr<Gtk::Builder>);
    static void check_traits();
    void on_toggle_bit(const int);
    void on_bitwise_op(std::function<uint32_t(uint32_t)>);

protected:
    void update_display();
    void update_bits();

    float extract_mantissa() { uint32_t m{s_zero_exp | (m_ivalue & s_mant_mask)}; return reinterpret_cast<float&>(m); }
    unsigned extract_exponent_biased() { return (m_ivalue & s_exp_mask) >> s_num_mant_bits; };
    int extract_exponent_unbiased() { return extract_exponent_biased() - s_bias; }
    bool extract_sign() { return (m_ivalue & s_sign_mask) != 0; }

    std::string format_value(float);
    std::string classify();
    __attribute__((unused)) void print_bits(uint32_t);

    static constexpr size_t s_num_bits{32};
    static constexpr size_t s_num_mant_bits{23};
    static constexpr size_t s_num_exp_bits{8};
    static constexpr size_t s_num_mantexp_bits{s_num_mant_bits + s_num_exp_bits};
    static constexpr uint32_t s_exp_mask{((1u<<s_num_exp_bits)-1) << s_num_mant_bits};
    static constexpr uint32_t s_mant_mask{(1u<<s_num_mant_bits)-1};
    static constexpr uint32_t s_sign_mask{1u<<s_num_mantexp_bits};
    static constexpr uint32_t s_zero_exp{((1u<<(s_num_exp_bits-1))-1) << s_num_mant_bits};
    static constexpr uint32_t s_max_denorm{(1u<<s_num_mant_bits)-1};
    static constexpr uint32_t s_bias{127};

    union {
        float m_fvalue;
        uint32_t m_ivalue;
    };

    std::array<Gtk::ToggleButton*, s_num_bits> m_bit_buttons;

    Gtk::Label *m_decval;
    Gtk::Label *m_signval;
    Gtk::Label *m_expval_biased;
    Gtk::Label *m_expval_unbiased;
    Gtk::Label *m_mantval;
    Gtk::Label *m_class;

    bool m_bitop_sign;
    bool m_bitop_exp;
    bool m_bitop_mant;

    using button_iterator = std::array<Gtk::ToggleButton*, s_num_bits>::iterator;

    const button_iterator m_mant_begin, m_mant_end;
    const button_iterator m_exp_begin, m_exp_end;
    const button_iterator m_sign_begin, m_sign_end;
};

#endif //IEEE754_IEEE754_H
