/*
 * A344836  Square array T(n, k), n, k >= 0, read by antidiagonals; T(n, k) =
 *          (n * 2^max(0, w(k)-w(n))) XOR (k * 2^max(0, w(n)-w(k))) (where XOR
 *          denotes the bitwise XOR operator and w = A070939).
 * https://oeis.org/A344836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) is the (n‑th) entry of the square array T(n,k) read * by
 *            antidiagonals, where * T(n,k) = (n * 2^max(0, w(k)-w(n))) XOR (k
 *            * 2^max(0, w(n)-w(k))), * with w(x) = number of binary digits of
 *            x (0 has w(0)=0)
 * Range:     u8 n <= 7089, u16 n <= 7089, u32 n <= 7089, u64 n <= 7089,
 *            u128 n <= 7089, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow2.h"

/* bit length of a non‑negative 64‑bit integer */
static inline unsigned bitlen_idx(idx_t v) {
    return v ? (64 - __builtin_clzll(v)) : 0;
}

seq_t a(idx_t n) {
    /* --- locate the antidiagonal containing index n --- */
    idx_t d = 0;
    while (1) {
        idx_t next = d + 1;
        __uint128_t cum = (__uint128_t)next * (next + 1) / 2;
        if (cum <= n) {
            d = next;
        } else {
            break;
        }
    }
    __uint128_t before128 = (__uint128_t)d * (d + 1) / 2;
    idx_t before = (idx_t)before128;
    idx_t offset = n - before;       /* 0 ≤ offset ≤ d */
    idx_t i = offset;
    idx_t j = d - offset;

    /* --- compute T(i,j) according to the definition --- */
    unsigned wi = bitlen_idx(i);   /* w(i) */
    unsigned wj = bitlen_idx(j);   /* w(j) */

    unsigned exp_i = (wj > wi) ? (wj - wi) : 0;   /* exponent for i */
    unsigned exp_j = (wi > wj) ? (wi - wj) : 0;   /* exponent for j */

    seq_t shift_i = exp_i ? pow2(exp_i) : (seq_t)1;
    seq_t shift_j = exp_j ? pow2(exp_j) : (seq_t)1;

    seq_t val = (seq_t)i * shift_i ^ (seq_t)j * shift_j;
    return val;
}
