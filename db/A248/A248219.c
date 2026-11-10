/*
 * A248219  Indices where A248218(n) = 1.
 * https://oeis.org/A248219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 1 such that the orbit of 0 under x -> x^2+1 mod
 *            m ends in a cycle of length 1 (A248218(m) = 1); found by search
 *            with Brent cycle detection
 * Range:     u8 n <= 15, u16 n <= 248, u32 n <= 717, u64 n <= 717,
 *            u128 n <= 717, big any n
 * Validated: standard, pass, 67 terms through n = 717 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A248219_period_one(uint64_t m)
{
    if (m == 1) return 1;
    if ((m & 1) == 0) return 0;
    if (m % 9 == 0) return 0;
    if (m % 3 == 2) return 0;
    {
        static const unsigned bad[] = {5, 11, 17, 23, 29, 41, 47, 53, 59, 71};
        for (unsigned i = 0; i < sizeof(bad) / sizeof(bad[0]); i++)
            if (m % bad[i] == 0) return 0;
    }
    uint64_t power = 1, lam = 1;
    uint64_t t = 0;
    uint64_t h = (1 % m);
    while (t != h) {
        if (power == lam) {
            t = h;
            power <<= 1;
            lam = 0;
        }
        h = (uint64_t)(((unsigned __int128)h * h + 1) % m);
        lam++;
    }
    return lam == 1;
}

static idx_t *A248219_terms = 0;
static size_t A248219_cnt = 0, A248219_cap = 0;
static idx_t A248219_next = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A248219_cnt < (size_t)n) {
        if (A248219_period_one((uint64_t)A248219_next)) {
            if (A248219_cnt == A248219_cap) {
                size_t nc = A248219_cap ? A248219_cap * 2 : 1024;
                idx_t *p = (idx_t *)realloc(A248219_terms, nc * sizeof(idx_t));
                if (!p) return 0;
                A248219_terms = p;
                A248219_cap = nc;
            }
            A248219_terms[A248219_cnt++] = A248219_next;
        }
        A248219_next++;
    }
    return (seq_t)A248219_terms[n - 1];
}
