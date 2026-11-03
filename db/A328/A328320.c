/*
 * A328320  Numbers for which A328311(n) = 1 + A051903(A003415(n)) -
 *          A051903(n) is zero (including 1 as the initial term).
 * https://oeis.org/A328320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 18133, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 90 terms through n = 8127 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A328320: numbers n for which 1 + A051903(A003415(n)) - A051903(n) == 0. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check whether m satisfies the property.  m is assumed > 1. */
static bool property_n(uint64_t m)
{
    uint64_t max_e = 0;
    uint64_t derivative = 0;
    uint64_t t = m;

    /* Factor m and accumulate derivative contributions. */
    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            uint8_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            if (e > max_e) max_e = e;
            derivative += (uint64_t)e * (m / p);
        }
    }
    if (t > 1) {          /* remaining prime factor */
        uint64_t p = t;
        uint8_t  e = 1;
        if (e > max_e) max_e = e;
        derivative += (uint64_t)e * (m / p);
    }

    /* Compute the maximum exponent in the factorization of the derivative. */
    uint64_t dmax_e = 0;
    uint64_t d = derivative;
    for (uint64_t p = 2; p <= d / p; ++p) {
        if (d % p == 0) {
            uint8_t e = 0;
            while (d % p == 0) {
                d /= p;
                ++e;
            }
            if (e > dmax_e) dmax_e = e;
        }
    }
    if (d > 1 && 1 > dmax_e) dmax_e = 1;

    return 1 + dmax_e == max_e;
}

/* Cached sequence of values. */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static uint64_t next_test = 0;   /* number to be tested next */

/* Ensure that seq contains at least n terms. */
static void ensure_terms(idx_t n)
{
    if (seq_len >= (size_t)n) return;

    if (seq_len == 0) {
        seq_cap = 16;
        seq = mem_new(seq_t, seq_cap);
        seq[0] = (seq_t)1;          /* 1 is always in the sequence */
        seq_len = 1;
        next_test = 2;              /* start testing from 2 */
    }

    while (seq_len < (size_t)n) {
        uint64_t m = next_test++;
        if (m == 1 || property_n(m)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = mem_resize(seq_t, seq, seq_cap);
            }
            seq[seq_len++] = (seq_t)m;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_terms(n);
    return seq[(size_t)n - 1];
}
