/*
 // Copyright (c) 2015-2016-2016 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include <iostream>
#include "test.hpp"
extern "C"
{
#include "../thread/src/thd.h"
}

#define XPD_TEST_NLOOP  16
#define XPD_TEST_NTHD   4

static xpd::mutex gmutex;

class post_tester : private xpd::instance
{
public:

    ~post_tester()
    {
        if(m_patch)
        {
            xpd::instance::close(m_patch);
        }
    }
    
    bool load(std::string const& name)
    {
        m_patch = xpd::instance::load(name, "");
        return bool(m_patch);
    }
    
    bool prepare(const int nins, const int nouts, const int samplerate, const int nsamples)
    {
        xpd::instance::prepare(nins, nouts, samplerate, nsamples);
        m_blksize = nsamples;
        return xpd::instance::samplerate() == samplerate;
    }
    
    void send(std::vector<xpd::atom> const& vec)
    {
        xpd::instance::send(m_tfrom, xpd::symbol("list"), vec);
    }
    
    void receive(xpd::console::post const& post) xpd_final
    {
        m_counter++;
    }
    
    size_t counter() const xpd_noexcept
    {
        return m_counter;
    }
    
    static void test(post_tester* inst)
    {
        gmutex.lock();
        for(size_t i = 0; i < XPD_TEST_NLOOP; i++)
        {
            inst->perform(inst->m_blksize, 0, NULL, 0, NULL);
        }
        gmutex.unlock();
    }
    
private:
    xpd::patch m_patch;
    xpd::tie   m_tfrom;
    xpd::tie   m_tto;
    size_t     m_counter;
    size_t     m_blksize;
};

TEST_CASE("instance", "[instance post]")
{
    post_tester inst[XPD_TEST_NTHD];
    thd_thread  thd[XPD_TEST_NTHD];
    
    SECTION("post")
    {
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            REQUIRE(inst[i].load("test_post.pd"));
            REQUIRE(inst[i].prepare(0, 0, 44100, 256));
            thd_thread_detach(thd+i, (thd_thread_method)(&post_tester::test), inst+i);
        }
        
        for(size_t i = 0; i < XPD_TEST_NTHD; ++i)
        {
            thd_thread_join(thd+i);
            CHECK(inst[i].counter() == XPD_TEST_NLOOP);
        }
    }
}

#undef XPD_TEST_NLOOP




