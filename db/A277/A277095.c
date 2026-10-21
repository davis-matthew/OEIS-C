/*
 * A277095  Numbers k such that sin(k) < 0 and sin(k+2) > 0.
 * https://oeis.org/A277095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + (n%2)) * π) - (n%2)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Integers k such that sin(k) < 0 and sin(k+2) > 0. */

#include <stdint.h>

static const seq_t PI_SCAL   = 3141592653589ULL;  /* floor(π * 1e12)   */
static const seq_t POW10_12  = 1000000000000ULL; /* 1e12              */

static inline seq_t floor_m_pi(idx_t m)
{
    /* floor(m * π) using fixed‑point arithmetic (12 decimal digits of π). */
    return ((seq_t)m * PI_SCAL) / POW10_12;
}

seq_t a(idx_t n)
{
    /* m = 2 * ceil(n/2)   (the even integer after n)   */
    idx_t m = n + (n & 1);
    seq_t val = floor_m_pi(m);
    return (n & 1) ? (val - 1) : val;   /* odd n -> val-1 , even n -> val  */
}
