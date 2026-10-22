/*
 * A392278  The maximum exponent in the prime factorization of the smallest
 *          multiple of n that is an exponentially squarefree number.
 * https://oeis.org/A392278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A392278: a(n) = the largest exponent in the prime factorisation of
   the smallest multiple of n that is exponentially squarefree.
   For each prime divisor p^e of n we replace e by the smallest
   square‑free integer >= e; the maximum of those values is a(n). */

#include <stdint.h>
#include <stdbool.h>

/* Test whether an integer is squarefree (i.e. no square > 1 divides it). */
static bool is_squarefree_uint64(uint64_t x)
{
    if (x == 0) return true;          /* 0 is treated as squarefree */
    for (uint64_t p = 2; p <= x / p; ++p) {
        if (x % p == 0) {
            uint64_t pp = p * p;
            if (x % pp == 0) return false;
        }
    }
    return true;
}

/* Return the smallest squarefree integer >= e. */
static uint64_t next_squarefree_ge(uint64_t e)
{
    uint64_t k = e;
    while (!is_squarefree_uint64(k)) ++k;
    return k;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0, and n=0 is defined as 0 as well */

    idx_t tmp = n;
    seq_t max_exp = 0;

    /* Factor 2 separately to speed up the loop a bit (optional). */
    if ((tmp & 1) == 0) {
        uint64_t exp = 0;
        while ((tmp & 1) == 0) {  /* while tmp is even */
            tmp >>= 1;
            ++exp;
        }
        uint64_t sf = next_squarefree_ge(exp);
        if (sf > max_exp) max_exp = sf;
    }

    /* Factor odd candidates. */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            uint64_t sf = next_squarefree_ge(exp);
            if (sf > max_exp) max_exp = sf;
        }
    }

    /* Any remaining prime factor > 1 has exponent 1. */
    if (tmp > 1) {
        /* exp = 1 is already squarefree, but we keep the logic consistent */
        uint64_t sf = next_squarefree_ge(1);
        if (sf > max_exp) max_exp = sf;
    }

    return max_exp;
}
