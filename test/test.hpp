/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../xpd/xpd.hpp"
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <cassert>

using namespace xpd;

//
// Bug with copy of atoms of symbols
//

#ifndef XPD_TEST_HPP
#define XPD_TEST_HPP

#define XPD_TEST_NLOOP 1

class type_test
{
public:
    
    static void test_tie()
    {
        tie t0;
        tie t1 = "zaza";
        tie t2 = std::string("zaza");
        tie t3 = t2;
        assert("test_tie 1" && !t0 && t1 && t1 == t2 && t2 == t3);
        assert("test_tie 2" && !t0 && t1.name() == "zaza" && t2.name() == "zaza" && t3.name() == "zaza");
        t0 = "zozo";
        t1 = std::string("blab");
        t2 = t0;
        t3 = t3;
        assert("test_tie " && t0 && t0 == t2 && t1 != t2 && t1 != t0 && t3 == t3);
        assert("test_tie 4" && t0.name() == "zozo" && t1.name() == "blab" && t2.name() == "zozo" && t3.name() == "zaza");
    }
    
    static void test_symbol()
    {
        symbol t0;
        symbol t1 = "zaza";
        symbol t2 = std::string("zaza");
        symbol t3 = t2;
        assert("test_tie 1" && !t0 && t1 && t1 == t2 && t2 == t3);
        assert("test_tie 2" && !t0 && t1.name() == "zaza" && t2.name() == "zaza" && t3.name() == "zaza");
        t0 = "zozo";
        t1 = std::string("blab");
        t2 = t0;
        t3 = t3;
        assert("test_tie " && t0 && t0 == t2 && t1 != t2 && t1 != t0 && t3 == t3);
        assert("test_tie 4" && t0.name() == "zozo" && t1.name() == "blab" && t2.name() == "zozo" && t3.name() == "zaza");
    }
    
};

class instance_test : private instance, public console::history
{
public:
    instance_test()
    {
        ;
    }
    
    ~instance_test()
    {
        ;
    }
    
    void receive(console::post const& post) xpd_final
    {
        console::history::add(post);
    }
    
    void receive(tie tie, symbol s, std::vector<atom> const& atoms) xpd_final
    {
        if(s == symbol::bang_s) {
            m_count_bang++;
        }
        else if(s == symbol::float_s) {
            m_count_float++;
        }
        else if(s == symbol::symbol_s) {
            m_count_symbol++;
        }
        else if(s == symbol::list_s) {
            m_count_list++;
        }
        else {
            m_count_anything++;
        }
        
        std::vector<atom> const& temp = m_atoms;
        assert("receive atoms s" && s == m_name);
        assert("receive atoms size" && atoms.size() == temp.size());
        
        int to_check_strange;
        /*
        for(size_t i = 0; i < atoms.size(); ++i)
        {
            assert("receive atoms type" && atoms[i].type() == temp[i].type());
            if(atoms[i].type() == atom::float_t)
            {
                assert("receive atoms type" && float(atoms[i]) == float(temp[i]));
            }
            else if(atoms[i].type() == atom::symbol_t)
            {
                assert("receive atoms type" && symbol(atoms[i]) == symbol(temp[i]));
            }
        }
         */
    }
    
    void receive(midi::event const& event)
    {
        if(event.type() == midi::event::note_t) {
            m_count_note++;
        }
        else if(event.type() == midi::event::control_change_t) {
            m_count_ctl++;
        }
        else if(event.type() == midi::event::program_change_t) {
            m_count_pgm++;
        }
        else if(event.type() == midi::event::pitch_bend_t) {
            m_count_bend++;
        }
        else if(event.type() == midi::event::after_touch_t) {
            m_count_after++;
        }
        else if(event.type() == midi::event::poly_after_touch_t) {
            m_count_poly++;
        }
        else {
            m_count_byte++;
        }
    }
    
    static void test_post(instance_test* inst)
    {
        inst->clear();
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            inst->send(console::post(console::log, "test"));
            inst->send(console::post(console::normal, "test"));
            inst->send(console::post(console::error, "test"));
            inst->send(console::post(console::fatal, "test"));
        }
        assert("test_post log" && inst->get_number_of_posts(console::log) == XPD_TEST_NLOOP);
        assert("test_post normal" && inst->get_number_of_posts(console::normal)  == XPD_TEST_NLOOP);
        assert("test_post error" && inst->get_number_of_posts(console::error) == XPD_TEST_NLOOP);
        assert("test_post fatal" && inst->get_number_of_posts(console::fatal) == XPD_TEST_NLOOP);
        
        assert("test_post log" && inst->get_number_of_posts_to_level(console::log) == XPD_TEST_NLOOP * 4);
        assert("test_post normal" && inst->get_number_of_posts_to_level(console::normal)  == XPD_TEST_NLOOP * 3);
        assert("test_post error" && inst->get_number_of_posts_to_level(console::error) == XPD_TEST_NLOOP * 2);
        assert("test_post fatal" && inst->get_number_of_posts_to_level(console::fatal) == XPD_TEST_NLOOP);
        
        inst->clear();
    }
    
    static void test_message(instance_test* inst)
    {
        char uid[512];
        patch* p = inst->load("test.pd", "");
        assert("test_message patch" && p);
        sprintf(uid, "%i", int(p->unique_id()));
        
        tie from(std::string(uid) + "-fromxpd");
        tie to1(std::string(uid) + "-toxpd1");
        tie to2(std::string(uid) + "-toxpd2");
        inst->bind(to1);
        inst->bind(to1);
        inst->bind(to2);
        
        inst->m_count_bang = 0;
        inst->m_count_float = 0;
        inst->m_count_symbol = 0;
        inst->m_count_list = 0;
        inst->m_count_anything = 0;
        inst->m_atoms.reserve(5);
        symbol zozo;
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            inst->m_name = symbol::bang_s;
            inst->m_atoms.clear();
            inst->send(from, inst->m_name, inst->m_atoms);
            
            inst->m_name = symbol::float_s;
            inst->m_atoms.push_back(1.f);
            assert("test_message float" && inst->m_atoms[0].type() == atom::float_t && float(inst->m_atoms[0]) == 1.f);
            inst->send(from, inst->m_name, inst->m_atoms);
            
           
            inst->m_name = symbol::symbol_s;
            zozo = "zozo";
            assert("test_message symbol" && zozo.name() == "zozo");
            inst->m_atoms[0] = zozo;
            assert("test_message symbol" && inst->m_atoms[0].type() == atom::symbol_t && symbol(inst->m_atoms[0]) == zozo);
            inst->send(from, inst->m_name, inst->m_atoms);
            
            inst->m_name = symbol::list_s;
            inst->m_atoms[0] = 1.f;
            inst->m_atoms.push_back(1.f);
            inst->m_atoms.push_back(zozo);
            inst->send(from, inst->m_name, inst->m_atoms);
            
            inst->m_name = symbol(std::string("zaza"));
            assert("test_message symbol" && inst->m_name.name() == "zaza");
            zozo =  std::string("zozo");
            assert("test_message symbol" && zozo.name() == "zozo");
            inst->m_atoms[0] = zozo;
            assert("test_message symbol" && inst->m_atoms[0].type() == atom::symbol_t && symbol(inst->m_atoms[0]) == zozo);
            inst->m_atoms[2] = "zuzu";
            assert("test_message symbol" && inst->m_atoms[2].type() == atom::symbol_t && symbol(inst->m_atoms[2]).name() == "zuzu");
            inst->m_atoms.push_back(symbol("zizi"));
            int to_see;
            //assert("test_message symbol" && inst->m_atoms[3].type() == atom::symbol_t && symbol(inst->m_atoms[3]).name() == "zizi");
            inst->send(from, inst->m_name, inst->m_atoms);
        }
        inst->unbind(to1);
        inst->unbind(to2);
        assert("test_message bang" && inst->m_count_bang == XPD_TEST_NLOOP * 2);
        assert("test_message float" && inst->m_count_float == XPD_TEST_NLOOP * 2);
        assert("test_message symbol" && inst->m_count_symbol == XPD_TEST_NLOOP * 2);
        assert("test_message list" && inst->m_count_list == XPD_TEST_NLOOP * 2);
        assert("test_message anything" && inst->m_count_anything == XPD_TEST_NLOOP * 2);
        inst->close(*p);
    }
    
    static void test_midi(instance_test* inst)
    {
        patch* p = inst->load("test.pd", "");
        assert("test_midi patch" && p);
        inst->m_count_note = 0;
        inst->m_count_ctl = 0;
        inst->m_count_pgm = 0;
        inst->m_count_bend = 0;
        inst->m_count_after = 0;
        inst->m_count_poly = 0;
        inst->m_count_byte = 0;
 
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            inst->send(midi::event::note(1, 1, 127));
            inst->send(midi::event::control_change(1, 1, 127));
            inst->send(midi::event::program_change(1, 1));
            inst->send(midi::event::pitch_bend(1, 1));
            inst->send(midi::event::after_touch(1, 1));
            inst->send(midi::event::poly_after_touch(1, 1, 1));
            inst->send(midi::event::byte(1));
        }

        assert("test_midi note" && inst->m_count_note == XPD_TEST_NLOOP);
        assert("test_midi ctl" && inst->m_count_ctl == XPD_TEST_NLOOP);
        assert("test_midi pgm" && inst->m_count_pgm == XPD_TEST_NLOOP);
        assert("test_midi bend" && inst->m_count_bend == XPD_TEST_NLOOP);
        assert("test_midi after" && inst->m_count_after == XPD_TEST_NLOOP);
        assert("test_midi poly" && inst->m_count_poly == XPD_TEST_NLOOP);
        assert("test_midi byte" && inst->m_count_byte == XPD_TEST_NLOOP);
        inst->close(*p);
    }
    
    
    static void test_dsp_int(instance_test* inst, int sr, int blks, int nins, int nouts)
    {
        inst->prepare(nins, nouts, sr, blks);
        assert("test_dsp sample rate" && inst->samplerate() == sr);
        
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            for(size_t k = 0; k < nins; k++)
            {
                for(size_t j = 0; j < blks; j++)
                {
                    inst->m_input[k][j] = ((rand() % 4000) - 2000.f) / 2000.f;
                }
            }
            inst->perform(blks, nins, const_cast<float const **>(inst->m_input), nouts, inst->m_output);
            for(size_t k = 0; k < nouts; k++)
            {
                for(size_t j = 0; j < blks; j++)
                {
                    assert("test_dsp samples" && inst->m_output[k][j] == inst->m_input[k][j]);
                }
            }
        }
    }
    
    static void test_dsp(instance_test* inst)
    {
        patch* p = inst->load("test.pd", "");
        assert("test_dsp patch" && p);
        inst->m_input[0] = new float[256];
        inst->m_input[1] = new float[256];
        inst->m_output[0] = new float[256];
        inst->m_output[1] = new float[256];
        assert("test_dsp inputs outputs" && inst->m_input[0] && inst->m_input[1] && inst->m_output[0] && inst->m_output[1]);
        
        test_dsp_int(inst, 44100, 64, 2, 2);
        test_dsp_int(inst, 44100, 256, 2, 1);
        test_dsp_int(inst, 96000, 128, 1, 1);
        
        delete [] inst->m_input[0];
        delete [] inst->m_input[1];
        delete [] inst->m_output[0];
        delete [] inst->m_output[1];
        inst->close(*p);
    }
    
    static void test_patch(instance_test* inst)
    {
        patch* p = inst->load("test.pd", "/home/maison");
        assert("test_patch patch" && !p);
        p = inst->load("test.pd", "");
        assert("test_patch patch" && p);
        assert("test_patch patch x" && p->x() == 100);
        assert("test_patch patch y" && p->y() == 100);
        assert("test_patch patch w" && p->width() == 85);
        assert("test_patch patch h" && p->height() == 60);
        
        int todo;
        std::vector<object*> objects(p->objects());
        inst->close(*p);
    }
    
private:
    float* m_input[2];
    float* m_output[2];
    symbol            m_name;
    std::vector<atom> m_atoms;
    
    size_t m_count_bang;
    size_t m_count_float;
    size_t m_count_symbol;
    size_t m_count_list;
    size_t m_count_anything;
    
    size_t m_count_note;
    size_t m_count_ctl;
    size_t m_count_pgm;
    size_t m_count_bend;
    size_t m_count_after;
    size_t m_count_poly;
    size_t m_count_byte;
};


#endif // XPD_TEST_HPP
