/*
 * A241924  16*s^8 - 168*s^4*t^4 + 9*t^8, where s > 0, t = 1..s.
 * https://oeis.org/A241924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 none, i16 n <= 2, i32 n <= 35, i64 n <= 1000, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241924:
   a(n) = 16*s^8 - 168*s^4*t^4 + 9*t^8,
   where the pair (s,t) is the n-th element of the
   triangular enumeration: s = 1,2,3,… and for each s,
   t runs from 1 to s.
   This generates the sequence in the order listed in the OEIS b-file. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the smallest s such that 1 + 2 + … + s ≥ n.
       Keep a running total to avoid overflow. */
    idx_t s = 1;
    idx_t total = 1;              /* sum up to current s */
    while (total < n) {
        ++s;
        total += s;
    }

    /* t = n - (total - s), where (total - s) is the sum up to s-1. */
    idx_t t = n - (total - s);

    /* Convert to seq_t for arithmetic. */
    seq_t ss = (seq_t)s;
    seq_t tt = (seq_t)t;

    /* Compute powers avoiding unnecessary large intermediates. */
    seq_t ss2 = ss * ss;
    seq_t ss4 = ss2 * ss2;
    seq_t ss8 = ss4 * ss4;

    seq_t tt2 = tt * tt;
    seq_t tt4 = tt2 * tt2;
    seq_t tt8 = tt4 * tt4;

    /* Apply the formula. */
    seq_t val = ((seq_t)16) * ss8
                - ((seq_t)168) * ss4 * tt4
                + ((seq_t)9)   * tt8;

    return val;
}
