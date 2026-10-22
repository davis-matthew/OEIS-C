/*
 * A367914  Movement sequence in the counter-clockwise undulating spiral,
 *          whereby 1, 2, 3, 4 represent moves to the right, down, left and
 *          up.
 * https://oeis.org/A367914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367914: Movement in the counter‑clockwise undulating spiral.
 *
 * The sequence is defined by four families of indices:
 *
 *   1. a(k)=1  when k = 8*i*i + 2*j + 1   with i,j >= 0 and j <= 4*i
 *   2. a(k)=2  when k = 8*i*i +12*i +2*j+6  with i,j >= 0 and j <= 4*i+3
 *   3. a(k)=3  when k = 8*i*i + 8*i +2*j+3  with i,j >= 0 and j <= 4*i+2
 *   4. a(k)=4  when k = 8*i*i + 4*i +2*j+2  with i,j >= 0 and j <= 4*i+1
 *
 * Every positive integer n appears exactly once in one of the four families,
 * and a(n) is that corresponding value.  The implementation below
 * iterates over the layers i until it finds the unique family that
 * generates n.  Since i grows roughly like sqrt(n/8), the loop is very
 * fast for all n <= 10000 (and much faster for all smaller values).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* values outside offset are defined as 0 */
        return (seq_t)0;
    }

    /* iterate over increasing i until n is matched */
    for (idx_t i = 0;; ++i) {
        /* use 128‑bit arithmetic to avoid overflow of intermediate values */
        unsigned __int128 ui = (unsigned __int128)i;
        unsigned __int128 base  = (unsigned __int128)8 * ui * ui;
        unsigned __int128 n128  = (unsigned __int128)n;

        /* quick bound: if the maximum index of layer i is still < n,
           the layer cannot contain n, so we skip to the next i.
           The maximum index in layer i is 8*i*i +20*i +12.            */
        unsigned __int128 max_i = base + (unsigned __int128)20 * ui + 12;
        if (max_i < n128) {
            continue;
        }

        /* --- family 1: value 1 ------------------------------------- */
        unsigned __int128 offset1 = base + 1;           /* 8*i*i + 1          */
        if (n128 >= offset1) {
            unsigned __int128 t = n128 - offset1;     /* 2*j                */
            if ((t & 1) == 0) {                       /* j must be integer  */
                unsigned __int128 j = t >> 1;
                if (j <= 4 * ui) {                    /* j <= 4*i          */
                    return (seq_t)1;
                }
            }
        }

        /* --- family 4: value 4 ------------------------------------- */
        unsigned __int128 offset4 = base + (unsigned __int128)4 * ui + 2; /* 8*i*i + 4*i + 2 */
        if (n128 >= offset4) {
            unsigned __int128 t = n128 - offset4;
            if ((t & 1) == 0) {
                unsigned __int128 j = t >> 1;
                if (j <= 4 * ui + 1) {
                    return (seq_t)4;
                }
            }
        }

        /* --- family 3: value 3 ------------------------------------- */
        unsigned __int128 offset3 = base + (unsigned __int128)8 * ui + 3; /* 8*i*i + 8*i + 3 */
        if (n128 >= offset3) {
            unsigned __int128 t = n128 - offset3;
            if ((t & 1) == 0) {
                unsigned __int128 j = t >> 1;
                if (j <= 4 * ui + 2) {
                    return (seq_t)3;
                }
            }
        }

        /* --- family 2: value 2 ------------------------------------- */
        unsigned __int128 offset2 = base + (unsigned __int128)12 * ui + 6; /* 8*i*i +12*i +6 */
        if (n128 >= offset2) {
            unsigned __int128 t = n128 - offset2;
            if ((t & 1) == 0) {
                unsigned __int128 j = t >> 1;
                if (j <= 4 * ui + 3) {
                    return (seq_t)2;
                }
            }
        }

        /* If we reach here, n was not found in any family of this i.
           The next i will be larger, and will contain n.  Continue the loop. */
    }
}
