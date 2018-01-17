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
#include "frequency_interleaver_vcc_impl.h"

namespace gr {
  namespace dab {

    frequency_interleaver_vcc::sptr
    frequency_interleaver_vcc::make(
            const std::vector<short> &interleaving_sequence) {
      return gnuradio::get_initial_sptr
              (new frequency_interleaver_vcc_impl(interleaving_sequence));
    }

    frequency_interleaver_vcc_impl::frequency_interleaver_vcc_impl(
            const std::vector<short> &interleaving_sequence)
            : gr::sync_block("frequency_interleaver_vcc",
                             gr::io_signature::make(1, 1, sizeof(gr_complex) *
                                                          interleaving_sequence.size()),
                             gr::io_signature::make(1, 1, sizeof(gr_complex) *
                                                          interleaving_sequence.size())),
              d_interleaving_sequence(interleaving_sequence),
              d_length(interleaving_sequence.size()) {
      for (unsigned int i = 0; i < d_length; i++)
        assert(d_interleaving_sequence[i] < (short) d_length);
    }


    int
    frequency_interleaver_vcc_impl::work(int noutput_items,
                                         gr_vector_const_void_star &input_items,
                                         gr_vector_void_star &output_items) {
      gr_complex const *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      for (int i = 0; i < noutput_items; i++) {
        for (unsigned int j = 0; j < d_length; j++)
          out[d_interleaving_sequence[j]] = in[j];
        out += d_length;
        in += d_length;
      }

      return noutput_items;
    }

  }
}

