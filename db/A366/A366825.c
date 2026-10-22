/*
 * A366825  Numbers of the form p^2 * m, squarefree m > 1, prime p < lpf(m),
 *          where lpf(m) = A020639(m).
 * https://oeis.org/A366825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A366825: numbers of the form p^2 * m where m is squarefree > 1
   and the least prime factor of m is greater than p.
   The function returns the nth term (1‑based index). */

#include <stdbool.h>
#include "../utils/lpf.h"

/* square‑free test for 64‑bit values */
static bool squarefree_uint64(uint64_t v)
{
    for (uint64_t d = 2; d * d <= v; ++d) {
        if (v % d == 0) {
            if ((v / d) % d == 0)              /* d^2 divides v */
                return false;
            while (v % d == 0) v /= d;         /* remove all factors of d */
        }
    }
    return true;                                 /* no repeated prime factor found */
}

/* test whether x satisfies the sequence condition */
static bool is_candidate(seq_t x)
{
    idx_t lp = lpf((idx_t)x);                /* smallest prime factor of x */
    seq_t p  = (seq_t)lp;
    seq_t p2 = p * p;
    if (x % p2 != 0)          /* p must divide x at least twice */
        return false;

    seq_t m = x / p2;         /* remaining part, must be > 1 */
    if (m <= (seq_t)1)
        return false;

    if (!squarefree_uint64((uint64_t)m))
        return false;

    idx_t lpf_m = lpf((idx_t)m);   /* least prime factor of m */
    return lp < lpf_m;             /* p must be strictly less than lpf(m) */
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n = 0, guard */

    idx_t count = 0;
    seq_t x = 12;                       /* first term is 12 */

    for (;; ++x) {
        if (is_candidate(x)) {
            ++count;
            if (count == n)
                return x;
        }
    }
}
