//
// Created by rhunter on 6/20/15.
//

#include <sstream>
#include <stdexcept>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <gtkmm/builder.h>
#include <gtkmm/label.h>
#include <gtkmm/checkbutton.h>
#include "Ieee754.hpp"

Ieee754::Ieee754(const Glib::RefPtr<Gtk::Builder> builder)
: m_ivalue{0},
  m_bitop_sign{false},
  m_bitop_exp{false},
  m_bitop_mant{false},
  m_mant_begin{m_bit_buttons.begin()},
  m_mant_end{m_bit_buttons.begin() + s_num_mant_bits},
  m_exp_begin{m_bit_buttons.begin() + s_num_mant_bits},
  m_exp_end{m_bit_buttons.begin() + s_num_mant_bits + s_num_exp_bits},
  m_sign_begin{m_bit_buttons.begin() + s_num_mant_bits + s_num_exp_bits},
  m_sign_end{m_bit_buttons.begin() + s_num_mant_bits + s_num_exp_bits + s_num_sign_bits}
{
    Ieee754::check_traits();

    for (int i=0; i< s_num_bits; ++i) {
        std::stringstream ss;
        ss << "toggleBit" << i;
        builder->get_widget(ss.str(), m_bit_buttons[i]);
        m_bit_buttons[i]->signal_toggled().connect([this,i](){on_toggle_bit(i);});
    }
    builder->get_widget("labelDecimalRepresentation", m_decval);
    builder->get_widget("labelSignValue", m_signval);
    builder->get_widget("labelExponentValueBiased", m_expval_biased);
    builder->get_widget("labelExponentValueUnbiased", m_expval_unbiased);
    builder->get_widget("labelMantissaValue", m_mantval);
    builder->get_widget("labelClass", m_class);


    {
        Gtk::Button *button;
        builder->get_widget("buttonBitwiseClear", button);
        auto op = [](uint32_t ival){return 0u;};
        button->signal_clicked().connect([this,op](){on_bitwise_op(op);});
    }
    {
        Gtk::Button *button;
        builder->get_widget("buttonBitwiseInvert", button);
        auto op = [](uint32_t ival){return ~ival;};
        button->signal_clicked().connect([this,op](){on_bitwise_op(op);});
    }
    {
        Gtk::Button *button;
        builder->get_widget("buttonBitwiseShiftLeft", button);
        auto op = [](uint32_t ival){return ival<<1;};
        button->signal_clicked().connect([this,op](){on_bitwise_op(op);});
    }
    {
        Gtk::Button *button;
        builder->get_widget("buttonBitwiseShiftRight", button);
        auto op = [](uint32_t ival){return ival>>1;};
        button->signal_clicked().connect([this,op](){on_bitwise_op(op);});
    }
    {
        Gtk::CheckButton *button;
        builder->get_widget("checkButtonSign", button);
        button->signal_clicked().connect([this](){ m_bitop_sign = !m_bitop_sign;});
    }
    {
        Gtk::CheckButton *button;
        builder->get_widget("checkButtonExponent", button);
        button->signal_clicked().connect([this](){ m_bitop_exp = !m_bitop_exp;});
    }
    {
        Gtk::CheckButton *b;
        builder->get_widget("checkButtonMantissa", b);
        b->signal_clicked().connect([this](){ m_bitop_mant = !m_bitop_mant;});
    }
    {
        Gtk::Button *b;
        builder->get_widget("buttonZero", b);
        b->signal_clicked().connect([this](){ m_fvalue = 0; update_bits();});
    }
    {
        Gtk::Button *b;
        builder->get_widget("buttonOne", b);
        b->signal_clicked().connect([this](){ m_fvalue = 1; update_bits();});
    }
    {
        Gtk::Button *b;
        builder->get_widget("buttonTen", b);
        b->signal_clicked().connect([this](){ m_fvalue = 10; update_bits();});
    }
    {
        Gtk::Button *b;
        builder->get_widget("buttonIncrement", b);
        b->signal_clicked().connect([this](){ m_fvalue += 1; update_bits();});
    }
    {
        Gtk::Button *b;
        builder->get_widget("buttonDecrement", b);
        b->signal_clicked().connect([this](){ m_fvalue -= 1; update_bits();});
    }
    {
        Gtk::Button *b;
        builder->get_widget("buttonDouble", b);
        b->signal_clicked().connect([this](){ m_fvalue *= 2; update_bits();});
    }
    {
        Gtk::Button *b;
        builder->get_widget("buttonHalve", b);
        b->signal_clicked().connect([this](){ m_fvalue *= 0.5; update_bits();});
    }
    {
        Gtk::Button *b;
        builder->get_widget("buttonInf", b);
        b->signal_clicked().connect([this](){ m_fvalue = std::numeric_limits<float>::infinity(); update_bits();});
    }
    {
        Gtk::Button *b;
        builder->get_widget("buttonNaN", b);
        b->signal_clicked().connect([this](){ m_fvalue = std::numeric_limits<float>::quiet_NaN(); update_bits();});
    }
    {
        Gtk::Button *b;
        builder->get_widget("buttonEpsilon", b);
        b->signal_clicked().connect([this](){ m_fvalue = std::numeric_limits<float>::epsilon(); update_bits();});
    }
    {
        Gtk::Button *b;
        builder->get_widget("buttonMaxNorm", b);
        b->signal_clicked().connect([this](){ m_fvalue = std::numeric_limits<float>::max(); update_bits();});
    }
    {
        Gtk::Button *b;
        builder->get_widget("buttonMinNorm", b);
        b->signal_clicked().connect([this](){ m_fvalue = std::numeric_limits<float>::min(); update_bits();});
    }
    {
        Gtk::Button *b;
        builder->get_widget("buttonMaxDenorm", b);
        b->signal_clicked().connect([this](){ m_ivalue = s_max_denorm; update_bits();});
    }
    {
        Gtk::Button *b;
        builder->get_widget("buttonMinDenorm", b);
        b->signal_clicked().connect([this](){ m_fvalue = std::numeric_limits<float>::denorm_min(); update_bits();});
    }
    update_display();
}

void Ieee754::check_traits()
{
    using float_limits = std::numeric_limits<float>;
    assert(float_limits::is_iec559);
}

template <typename T>
inline std::string Ieee754::format_value(T value)
{
    std::stringstream ss;
    ss << std::setprecision(std::numeric_limits<float>::digits10 + 1) << std::scientific << value;
    return ss.str();
}

void Ieee754::on_toggle_bit(const int idx) {

    auto label = dynamic_cast<Gtk::Label*>(m_bit_buttons[idx]->get_child());

    if (m_bit_buttons[idx]->get_active()) {
        m_ivalue |= 1u << idx;
        label->set_text("1");
    } else {
        m_ivalue &= ~(1u << idx);
        label->set_text("0");
    }
    update_display();
}

void Ieee754::update_display()
{
    m_decval->set_text(format_value<float>(m_fvalue));

    m_signval->set_text(m_ivalue & (1u<<31) ? "-" : "+");

    const uint32_t ev{(m_ivalue & s_exp_mask) >> 23};
    m_expval_biased->set_text(format_value<int32_t>(ev));
    m_expval_unbiased->set_text(format_value<int32_t>(ev - 127));

    const float mv{value_to_float(s_zero_exp | (m_ivalue & ((1u<<23)-1)))};
    m_mantval->set_text(format_value<float>(mv));

    m_class->set_text(classify());
}

void Ieee754::update_bits()
{
    uint32_t bitmask{1u};

    for (auto& b: m_bit_buttons) {
        b->set_active((m_ivalue & bitmask) != 0);
        bitmask <<= 1;
    }
}

void Ieee754::on_bitwise_op(uint32_t(*op)(uint32_t))
{
    uint32_t bitmask{1u};
    auto sval = op(m_ivalue);

    for (button_iterator it = m_mant_begin; it != m_mant_end; ++it, bitmask<<=1) {
        if (m_bitop_mant) {
            (*it)->set_active((sval & bitmask) != 0);
        }
    }
    for (button_iterator it = m_exp_begin; it != m_exp_end; ++it, bitmask<<=1) {
        if (m_bitop_exp) {
            (*it)->set_active((sval & bitmask) != 0);
        }
    }
    for (button_iterator it = m_sign_begin; it != m_sign_end; ++it, bitmask<<=1) {
        if (m_bitop_sign) {
            (*it)->set_active((sval & bitmask) != 0);
        }
    }
}

std::string Ieee754::classify()
{
    switch (std::fpclassify(m_fvalue)) {
        case FP_INFINITE:  return "Inf";
        case FP_NAN:       return "NaN";
        case FP_NORMAL:    return "normal";
        case FP_SUBNORMAL: return "subnormal";
        case FP_ZERO:      return "zero";
        default:           return "unknown";
    }
}

__attribute__((unused))
void Ieee754::print_val(uint32_t ival)
{
    for (int i = 31; i >= 0; --i) {
        if ((ival & (1u<<i)) != 0) {
            std::cout << 1;
        } else {
            std::cout << 0;
        }
    }
    std::cout << std::endl;
}