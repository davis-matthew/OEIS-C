/*
 * A342081  Numbers without an inferior odd divisor > 1.
 * https://oeis.org/A342081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the nth smallest integer that has no inferior odd divisor >1.  
   Equivalently, a(n) is the nth integer that is either a power of 2 or of the form 2^k * p
   where p is an odd prime with p > 2^k. */

#include <stdbool.h>

/* Test whether a small (fits in idx_t) integer is prime. */
static inline bool is_prime_small(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* Check if m has no inferior odd divisor > 1.  All operations are on seq_t. */
static inline bool qualifies(seq_t m)
{
    if (m == (seq_t)1) return true;                 /* 1 is allowed */

    /* Factor out powers of two: m = 2^k * r with r odd. */
    seq_t r = m;
    idx_t   k = 0;
    while ((r & (seq_t)1) == (seq_t)0) {            /* r even */
        r /= (seq_t)2;
        k++;
    }

    if (r == (seq_t)1) return true;                 /* pure power of two */

    /* r must be prime and larger than 2^k. */
    idx_t r_small = (idx_t)r;                       /* safe: r is small enough */
    if (!is_prime_small(r_small)) return false;

    seq_t two_pow_k = ((seq_t)1) << k;               /* 2^k */
    return r > two_pow_k;
}

/* Return the nth term (n >= 1) of sequence A342081. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cur = (seq_t)1;

    while (true) {
        if (qualifies(cur)) {
            count++;
            if (count == n) return cur;
        }
        ++cur;
    }
}
