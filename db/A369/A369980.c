/*
 * A369980  Run lengths in A369001.
 * https://oeis.org/A369980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run of equal terms in A369001, where
 *            A369001(m) = 1 if m'/gcd(m,m') is even, else 0 (m' =
 *            A003415(m)); computed by scanning A369001 and caching the run
 *            lengths
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9911 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A369980_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static int A369980_f(uint64_t m) {
    if (m <= 1) return 1;            /* m'=0, 0/1 = 0 is even */
    uint64_t rem = m, d = 0, p;
    for (p = 2; p * p <= rem; p += (p == 2 ? 1 : 2)) {
        if (rem % p == 0) {
            uint64_t e = 0, pe = 1;
            while (rem % p == 0) { rem /= p; e++; pe *= p; }
            d += (m / pe) * (e * (pe / p));
        }
    }
    if (rem > 1) d += m / rem;
    uint64_t g = A369980_gcd(m, d);
    return ((d / g) % 2 == 0) ? 1 : 0;
}

static uint32_t *A369980_runs = 0;
static size_t A369980_cnt = 0, A369980_cap = 0;
static uint64_t A369980_next = 1;
static int A369980_cv = 0;
static uint32_t A369980_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while ((uint64_t)A369980_cnt < (uint64_t)n) {
        uint64_t m = A369980_next++;
        int v = A369980_f(m);
        if (m == 1) { A369980_cv = v; A369980_len = 1; }
        else if (v == A369980_cv) A369980_len++;
        else {
            if (A369980_cnt == A369980_cap) {
                A369980_cap = A369980_cap ? A369980_cap * 2 : 1024;
                A369980_runs = (uint32_t *)realloc(A369980_runs, A369980_cap * sizeof(uint32_t));
            }
            A369980_runs[A369980_cnt++] = A369980_len;
            A369980_cv = v;
            A369980_len = 1;
        }
    }
    return (seq_t)A369980_runs[n - 1];
}
