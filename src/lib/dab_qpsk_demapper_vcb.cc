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

#include <dab_qpsk_demapper_vcb.h>
#include <gr_io_signature.h>

/*
 * Create a new instance of dab_qpsk_demapper_vcb and return
 * a boost shared_ptr.  This is effectively the public constructor.
 */
dab_qpsk_demapper_vcb_sptr 
dab_make_qpsk_demapper_vcb (int symbol_length)
{
  return dab_qpsk_demapper_vcb_sptr (new dab_qpsk_demapper_vcb (symbol_length));
}

dab_qpsk_demapper_vcb::dab_qpsk_demapper_vcb (int symbol_length) : 
  gr_sync_block ("qpsk_demapper_vcb",
             gr_make_io_signature (1, 1, sizeof(gr_complex)*symbol_length),
             gr_make_io_signature (1, 1, sizeof(char)*symbol_length/4)),
  d_symbol_length(symbol_length)
{
  assert(symbol_length>0);
  assert(symbol_length%4==0);
}


int 
dab_qpsk_demapper_vcb::work (int noutput_items,
                        gr_vector_const_void_star &input_items,
                        gr_vector_void_star &output_items)
{
  gr_complex const *in = (const gr_complex *) input_items[0];
  char *out = (char *) output_items[0];

  char byte = 0;

  for (int i=0; i<noutput_items; i++) {
    for (int j=0; j<d_symbol_length/4; j++) {
      byte = 0;
      for (int k=0; k<4; k++) {
        if (in[4*j+k].real()<0)
          byte+=1;
        byte = byte << 1;
        if (in[4*j+k].imag()<0)
          byte+=1;
        if (k<3)
          byte = byte << 1;
      }
      out[j] = byte;
    }
    in  += d_symbol_length;
    out += d_symbol_length/4;
  }
  
  return noutput_items;
}
