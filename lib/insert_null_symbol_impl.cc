/* -*- c++ -*- */
/*
 * Copyright 2004 Free Software Foundation, Inc.
 * 
 * This file is part of GNU Radio
 * 
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

/*
 * config.h is generated by configure.  It contains the results
 * of probing for features, options etc.  It should be the first
 * file included in your .cc file.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "insert_null_symbol_impl.h"

namespace gr {
  namespace dab {

    insert_null_symbol::sptr
    insert_null_symbol::make(int ns_length, int symbol_length) {
      return gnuradio::get_initial_sptr
              (new insert_null_symbol_impl(ns_length, symbol_length));
    }


    insert_null_symbol_impl::insert_null_symbol_impl(int ns_length,
                                                     int symbol_length)
            : gr::block("insert_null_symbol",
                        gr::io_signature::make2(2, 2, sizeof(gr_complex) *
                                                      symbol_length,
                                                sizeof(char)),
                        gr::io_signature::make(1, 1, sizeof(gr_complex))),
              d_ns_length(ns_length), d_symbol_length(symbol_length),
              d_ns_added(0) {
      /* note: setting output_multiple to a high value without setting the relative rate produces
       *
       * gr_vmcircbuf_sysv_shm: shmget (1): Invalid argument
       * gr_buffer::allocate_buffer: failed to allocate buffer of size 102080 KB
       * terminate called after throwing an instance of 'std::bad_alloc'
       *   what():  St9bad_alloc
       *
       * maybe the scheduler thinks that d_symbol_length input items are needed, which uses a lot of memory
       **/
      set_output_multiple(d_symbol_length);
      set_relative_rate(d_symbol_length);
    }

    void
    insert_null_symbol_impl::forecast(int noutput_items,
                                      gr_vector_int &ninput_items_required) {
      int in_req = noutput_items / d_symbol_length;
      unsigned ninputs = ninput_items_required.size();
      for (unsigned i = 0; i < ninputs; i++)
        ninput_items_required[i] = in_req;
    }


    int
    insert_null_symbol_impl::general_work(int noutput_items,
                                          gr_vector_int &ninput_items,
                                          gr_vector_const_void_star &input_items,
                                          gr_vector_void_star &output_items) {
      const gr_complex *iptr = (const gr_complex *) input_items[0];
      const char *trigger = (const char *) input_items[1];

      gr_complex *optr = (gr_complex *) output_items[0];

      int produced_items = 0;
      int consumed_items = 0;
      int i;

      while (noutput_items - produced_items >= d_symbol_length &&
             consumed_items < ninput_items[0] &&
             consumed_items < ninput_items[1]) {
        if (*trigger == 1 && d_ns_added < d_ns_length) {
          for (i = 0; i < d_ns_length - d_ns_added &&
                      i < noutput_items - produced_items; i++)
            *optr++ = 0;
          produced_items += i;
          d_ns_added += i;
        } else {
          if (*trigger == 0)
            d_ns_added = 0;
          for (int i = 0; i < d_symbol_length; i++)
            *optr++ = *iptr++;
          produced_items += d_symbol_length;
          trigger++;
          consumed_items++;
        }
      }

      consume_each(consumed_items);
      return produced_items;
    }

  }
}
