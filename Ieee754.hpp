//
// Created by rhunter on 6/20/15.
//

#ifndef IEEE754_IEEE754_H
#define IEEE754_IEEE754_H

#include <cstdint>
#include <gtkmm/togglebutton.h>

class Ieee754
{

public:
    Ieee754(const Glib::RefPtr<Gtk::Builder>);
    static void check_traits();
    void on_toggle_bit(const int);
    void on_bitwise_op(uint32_t (*)(uint32_t));
    void update_bits();

protected:
    void update_display();
    //uint32_t value_to_int() { return reinterpret_cast<uint32_t&>(m_value); }
    float value_to_float(uint32_t ival) { return reinterpret_cast<float&>(ival); }
    template <typename T> std::string format_value(T);
    std::string classify();
    __attribute__((unused)) void print_val(uint32_t ival);

    static constexpr size_t s_num_bits{32};
    static constexpr size_t s_num_mant_bits{23};
    static constexpr size_t s_num_exp_bits{8};
    static constexpr size_t s_num_sign_bits{1};
    static constexpr uint32_t s_exp_mask{((1u<<8)-1) << 23};
    static constexpr uint32_t s_zero_exp{((1u<<7)-1) << 23};
    static constexpr uint32_t s_max_denorm{(1u<<23)-1};

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
    const button_iterator m_mant_begin, m_mant_end, m_exp_begin, m_exp_end, m_sign_begin, m_sign_end;
};


#endif //IEEE754_IEEE754_H
