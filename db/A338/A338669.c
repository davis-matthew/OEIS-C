/*
 * A338669  The prime tower factorization of a(n) is obtained by replacing the
 *          rightmost prime number by 1 in the prime tower factorization of n;
 *          a(1) = 1.
 * https://oeis.org/A338669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1. For n > 1, let p be the largest prime factor of n and
 *            e be the exponent of p in the prime factorisation of n. Then
 *            a(n) = n / p^(e - a(e)) if e > 1 a(n) = n / p if e == 1 where
 *            a(e) is evaluated recursively. This implements the term‑function
 *            of OEIS A338669
 * Range:     u8 n <= 377, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9756 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes from the common utils */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"

/* Find largest prime factor of n and its exponent.
   n is guaranteed > 1. */
static void largest_prime_factor_exponent(idx_t n, uint64_t *p_out, uint64_t *e_out)
{
    uint64_t tmp = (uint64_t)n;
    uint64_t max_p = 0;
    uint64_t max_e = 0;

    /* factor 2 first */
    if ((tmp & 1) == 0) {
        uint64_t e = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++e;
        }
        max_p = 2;
        max_e = e;
    }

    /* trial division by odd numbers */
    for (uint64_t d = 3; d * d <= tmp; d += 2) {
        if (tmp % d == 0) {
            uint64_t e = 0;
            while (tmp % d == 0) {
                tmp /= d;
                ++e;
            }
            if (d > max_p) {
                max_p = d;
                max_e = e;
            }
        }
    }

    /* remaining tmp is either 1 or a prime > sqrt(original n) */
    if (tmp > 1) {
        if (tmp > max_p) {
            max_p = tmp;
            max_e = 1;
        }
    }

    *p_out = max_p;
    *e_out = max_e;
}

/* OEIS A338669 sequence term */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;                /* base case */

    uint64_t p, e;
    largest_prime_factor_exponent(n, &p, &e);

    if (e == 1)                         /* n is a prime or has a prime factor with exponent 1 */
        return ((seq_t)n) / ((seq_t)p);

    /* recursive step */
    seq_t a_e = a((idx_t)e);
    uint64_t a_e_uint = (uint64_t)a_e;   /* a(e) fits in 64‑bit because e ≤ log₂(n) ≤ 64 */
    uint64_t delta = e - a_e_uint;       /* e > a_e, so delta ≥ 1 */
    seq_t pow = ipow_idx((seq_t)p, (idx_t)delta);
    return ((seq_t)n) / pow;
}
