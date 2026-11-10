/*
 * A340324  Numbers k such that starting with k and repeatedly applying the
 *          map x -> A340323(x) reaches the loop {3, 4}.
 * https://oeis.org/A340324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (increasing) such that iterating x -> A340323(x)
 *            from k reaches the loop {3,4}; found by a search with
 *            memoization
 * Range:     u8 n <= 20, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A340324_step(uint64_t m)
{
    uint64_t r = 1;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            uint64_t t = p + 1;
            while (--e > 0) t *= (p - 1);
            r *= t;
        }
    }
    if (m > 1) r *= (m + 1);
    return r;
}

static bool A340324_reaches(uint64_t k)
{
    uint64_t x = k;
    for (int i = 0; i < 200; ++i) {
        if (x == 3 || x == 4) return true;
        if (x == 1) return false;
        x = A340324_step(x);
    }
    return false;
}

#define A340324_CAP 4096
static uint64_t A340324_terms[A340324_CAP];
static idx_t A340324_cnt = 0;
static uint64_t A340324_next = 2;

seq_t a(idx_t n)
{
    while (A340324_cnt < n && A340324_cnt < A340324_CAP) {
        if (A340324_reaches(A340324_next))
            A340324_terms[A340324_cnt++] = A340324_next;
        ++A340324_next;
    }
    return (seq_t)A340324_terms[n - 1];
}
