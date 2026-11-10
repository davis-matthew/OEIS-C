/*
 * A363171  Numbers k such that A064549(k) is an abundant number (A005101).
 * https://oeis.org/A363171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A064549(k) = k*rad(k) is abundant, i.e.
 *            sigma(k*rad(k)) > 2*k*rad(k); computed by search with
 *            memoization
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9913 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool A363171_good(uint64_t k)
{
    seq_t sig = 1, m = 1;
    uint64_t t = k;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            seq_t pw = 1, s = 1;
            while (t % p == 0) {
                t /= p;
                pw *= (seq_t)p;
                s += pw;
            }
            pw *= (seq_t)p;   /* p^(e+1) */
            s += pw;
            sig *= s;
            m *= pw;
        }
    }
    if (t > 1) {
        seq_t p = (seq_t)t;
        sig *= (1 + p + p * p);
        m *= p * p;
    }
    return sig > 2 * m;
}

static uint64_t *A363171_terms = 0;
static uint64_t A363171_cnt = 0, A363171_cap = 0, A363171_next = 1;

seq_t a(idx_t n)
{
    while (A363171_cnt < (uint64_t)n) {
        uint64_t k = A363171_next++;
        if (A363171_good(k)) {
            if (A363171_cnt == A363171_cap) {
                A363171_cap = A363171_cap ? A363171_cap * 2 : 1024;
                A363171_terms = (uint64_t *)realloc(A363171_terms, A363171_cap * sizeof(uint64_t));
            }
            A363171_terms[A363171_cnt++] = k;
        }
    }
    return (seq_t)A363171_terms[n - 1];
}
