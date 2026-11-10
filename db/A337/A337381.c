/*
 * A337381  Numbers k for which A003973(k) >= 2*sigma(k).
 * https://oeis.org/A337381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (increasing) such that A003973(k) >= 2*sigma(k);
 *            found by search, with A003973 and sigma both multiplicative over
 *            the factorization of k
 * Range:     u8 n <= 14, u16 n <= 898, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 89 terms through n = 7231 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static bool A337381_test(uint64_t k) {
    seq_t s = 1, t = 1;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            seq_t ps = 1, pp = 1, qs = 1, qp = 1;
            while (m % p == 0) {
                m /= p;
                pp *= (seq_t)p; ps += pp;
                qp *= (seq_t)q; qs += qp;
            }
            s *= ps;
            t *= qs;
        }
    }
    if (m > 1) {
        s *= ((seq_t)m + 1);
        t *= ((seq_t)next_prime(m) + 1);
    }
    return t >= 2 * s;
}

static uint64_t *A337381_tbl = 0;
static uint64_t A337381_cnt = 0, A337381_cap = 0, A337381_k = 0;

seq_t a(idx_t n) {
    while (A337381_cnt < (uint64_t)n) {
        A337381_k++;
        if (A337381_k >= 2 && A337381_test(A337381_k)) {
            if (A337381_cnt == A337381_cap) {
                A337381_cap = A337381_cap ? A337381_cap * 2 : 1024;
                A337381_tbl = (uint64_t *)realloc(A337381_tbl, A337381_cap * sizeof(uint64_t));
            }
            A337381_tbl[A337381_cnt++] = A337381_k;
        }
    }
    return (seq_t)A337381_tbl[n - 1];
}
