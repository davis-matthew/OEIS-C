/*
 * A292239  A multiplicative encoding for the exponents of 2 obtained when
 *          using Shevelev's algorithm for computing A002326.
 * https://oeis.org/A292239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..k} prime(v_i), where v_i are the 2-adic
 *            valuations of x_i + (2n+1) along the cycle x -> odd_part(x +
 *            2n+1) starting at x = 1 (k = A179382(n+1)); by direct simulation
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 56 terms through n = 958 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A292239_pr = NULL;
static size_t A292239_cnt = 0, A292239_cap = 0;

static uint64_t A292239_prime(size_t k) {
    /* k-th prime, 1-indexed */
    if (A292239_cap == 0) {
        A292239_cap = 64;
        A292239_pr = (uint64_t *)malloc(A292239_cap * sizeof(uint64_t));
    }
    uint64_t cand = A292239_cnt ? A292239_pr[A292239_cnt - 1] + 1 : 2;
    while (A292239_cnt < k) {
        int isp = 1;
        for (size_t i = 0; i < A292239_cnt && A292239_pr[i] * A292239_pr[i] <= cand; i++) {
            if (cand % A292239_pr[i] == 0) { isp = 0; break; }
        }
        if (isp) {
            if (A292239_cnt == A292239_cap) {
                A292239_cap *= 2;
                A292239_pr = (uint64_t *)realloc(A292239_pr, A292239_cap * sizeof(uint64_t));
            }
            A292239_pr[A292239_cnt++] = cand;
        }
        cand++;
    }
    return A292239_pr[k - 1];
}

seq_t a(idx_t n) {
    seq_t m = (seq_t)n * 2 + 1;
    seq_t cur = 1;
    seq_t res = 1;
    do {
        seq_t t = cur + m;
        size_t v = 0;
        while ((t & 1) == 0) { t >>= 1; v++; }
        cur = t;
        res *= (seq_t)A292239_prime(v);
    } while (cur != 1);
    return res;
}
