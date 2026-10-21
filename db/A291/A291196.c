/*
 * A291196  Third differences of {Pi*n^2}, fractional part of Pi*n^2.
 * https://oeis.org/A291196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -F(n+3) + 3*F(n+2) - 3*F(n+1) + F(n) where F(k) = floor(π
 *            * k^2)
 * Range:     i8 none, i16 none, i32 n <= 1, i64 n <= 10000, i128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 10^16 scaling of π:  floor(π * 10^16) = 31415926535897932  */
static const seq_t PI10   = (seq_t)31415926535897932ULL;
static const seq_t SCALE10 = (seq_t)10000000000000000ULL;

/*  Compute floor(π * n^2) using the 10^16 scaled integer approximation
    of π.  The product uses a 128‑bit intermediate when available. */
static seq_t floor_pi_sq(idx_t n)
{
#ifdef __SIZEOF_INT128__
    __int128 pr = (__int128)PI10 * (__int128)n * (__int128)n;
    return (seq_t)(pr / SCALE10);
#else
    seq_t nn   = (seq_t)n * (seq_t)n;
    seq_t prod = (seq_t)PI10 * nn;
    return prod / SCALE10;
#endif
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    seq_t f0 = floor_pi_sq(n);
    seq_t f1 = floor_pi_sq(n + 1);
    seq_t f2 = floor_pi_sq(n + 2);
    seq_t f3 = floor_pi_sq(n + 3);
    return -f3 + 3 * f2 - 3 * f1 + f0;
}
