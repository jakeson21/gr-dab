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

#include <dab_moving_sum_cc.h>
#include <gr_io_signature.h>

/*
 * Create a new instance of dab_moving_sum_cc and return
 * a boost shared_ptr.  This is effectively the public constructor.
 */
dab_moving_sum_cc_sptr 
dab_make_moving_sum_cc (int length)
{
  return dab_moving_sum_cc_sptr (new dab_moving_sum_cc (length));
}

/*
 * Specify constraints on number of input and output streams.
 * This info is used to construct the input and output signatures
 * (2nd & 3rd args to gr_block's constructor).  The input and
 * output signatures are used by the runtime system to
 * check that a valid number and type of inputs and outputs
 * are connected to this block.  In this case, we accept
 * only 1 input and 1 output.
 */
static const int MIN_IN = 1;  // mininum number of input streams
static const int MAX_IN = 1;  // maximum number of input streams
static const int MIN_OUT = 1; // minimum number of output streams
static const int MAX_OUT = 1; // maximum number of output streams

/*
 * The private constructor
 */
dab_moving_sum_cc::dab_moving_sum_cc (int length)
  : gr_sync_block ("moving_sum_cc",
       gr_make_io_signature (MIN_IN, MAX_IN, sizeof (gr_complex)),
       gr_make_io_signature (MIN_OUT, MAX_OUT, sizeof (gr_complex))),
    d_length(length), d_sum(0)
{
  assert(length>=0);
  set_history(length+1);
}

/*
 * Our virtual destructor.
 */
dab_moving_sum_cc::~dab_moving_sum_cc ()
{
  // nothing else required in this example
}

int 
dab_moving_sum_cc::work (int noutput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items)
{
  const gr_complex *in = (const gr_complex *) input_items[0];
  const gr_complex *in_shifted = &in[d_length];
  gr_complex *out = (gr_complex *) output_items[0];
  for (int i=0; i < noutput_items; i++) {
    d_sum+=(gr_complexd)in[i+d_length]-(gr_complexd)in[i];
    out[i] = (gr_complex)d_sum;
  }

  // Tell runtime system how many output items we produced.
  return noutput_items;
}
