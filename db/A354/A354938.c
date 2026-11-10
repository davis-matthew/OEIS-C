/*
 * A354938  Row 8 of A354940: Numbers k for which A345992(k) = 8, divided by
 *          8.
 * https://oeis.org/A354938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k/8 where k is the n-th number with A345992(k) = 8. Found
 *            by a search over j = 1, 2, ... (k = 8j) with memoization;
 *            A345992(k) is the unitary divisor d of k that gives the smallest
 *            m with k | m(m+1) (m = 0 mod d, m = -1 mod k/d), and the value
 *            is gcd(k, m) = d
 * Range:     u8 n <= 46, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static int64_t A354938_inv(int64_t a, int64_t mod) {
    int64_t r0 = mod, r1 = a % mod, t0 = 0, t1 = 1;
    while (r1 != 0) {
        int64_t q = r0 / r1;
        int64_t t = r0 - q * r1; r0 = r1; r1 = t;
        t = t0 - q * t1; t0 = t1; t1 = t;
    }
    t0 %= mod;
    if (t0 < 0) t0 += mod;
    return t0;
}

/* A345992(k): gcd(k, m) for the smallest positive m with k | m(m+1) */
static uint64_t A354938_a345992(uint64_t k) {
    uint64_t pp[20];
    int c = 0;
    uint64_t x = k;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            uint64_t pe = 1;
            while (x % p == 0) { x /= p; pe *= p; }
            pp[c++] = pe;
        }
    }
    if (x > 1) pp[c++] = x;
    uint64_t bestm = k, bestd = k;
    for (unsigned mask = 0; mask < (1u << c); ++mask) {
        uint64_t d = 1;
        for (int i = 0; i < c; ++i) if (mask & (1u << i)) d *= pp[i];
        uint64_t e = k / d;
        uint64_t m;
        if (e == 1) m = k;
        else {
            uint64_t inv = (uint64_t)A354938_inv((int64_t)(d % e), (int64_t)e);
            uint64_t t = (e - inv) % e;
            m = d * t;
            if (m == 0) m = k;
        }
        if (m < bestm) { bestm = m; bestd = d; }
    }
    return bestd;
}

static uint64_t *A354938_found = NULL;
static uint64_t A354938_cnt = 0, A354938_cap = 0, A354938_j = 0;

seq_t a(idx_t n) {
    while (A354938_cnt < (uint64_t)n) {
        ++A354938_j;
        if (A354938_a345992(8 * A354938_j) == 8) {
            if (A354938_cnt == A354938_cap) {
                A354938_cap = A354938_cap ? A354938_cap * 2 : 256;
                A354938_found = (uint64_t *)realloc(A354938_found, A354938_cap * sizeof(uint64_t));
            }
            A354938_found[A354938_cnt++] = A354938_j;
        }
    }
    return (seq_t)A354938_found[n - 1];
}
