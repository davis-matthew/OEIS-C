/*
 * A320562  Irregular table read by rows: T(n,k) is the smallest m such that
 *          m^m == 2*k + 1 (mod 2^n), 0 <= k <= 2^(n-1) - 1.
 * https://oeis.org/A320562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = read of the irregular table T(n,k) where T(n,k) is the *
 *            smallest odd m such that m^m == 2*k+1 (mod 2^n)
 * Range:     u8 n <= 63, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   floor_log2_uint64, mod_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/floor_log2_uint64.h"
#include "../utils/mod_pow.h"

/* Recursively compute T(n,k).  n>=1, 0<=k<2^(n-1). */
static seq_t T_row(idx_t n, idx_t k)
{
    if (n == 1) {
        return (seq_t)1;
    }

    /* 2^(n-2) */
    idx_t halfk = ((idx_t)1ULL << (n - 2));

    if (k < halfk) {
        /* First regime: compare T(n-1,k)^T(n-1,k) with 2*k+1 modulo 2^n. */
        seq_t t = T_row(n - 1, k);
        seq_t mod = pow2(n);                       /* 2^n */
        seq_t target = (seq_t)((2ULL * k) + 1);
        /* exp fits into idx_t for n <= 63 (our use‑case). */
        seq_t val = mod_pow(t, (idx_t)t, mod);
        if (val == target) {
            return t;
        } else {
            return t + pow2(n - 1);
        }
    } else {
        /* Second regime: use the symmetry of the table. */
        idx_t k2 = k - halfk;
        seq_t t = T_row(n, k2);
        seq_t half = pow2(n - 1);
        if (t < half) {
            return t + half;
        } else {
            return t - half;
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Sequence is 1‑based; return 0 for n==0 (undefined). */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Row index r such that 2^(r-1) <= n <= 2^r-1. */
    idx_t r = floor_log2_uint64(n) + 1;

    /* Position k within the row: 0 <= k < 2^(r-1). */
    idx_t k = n - ((idx_t)1ULL << (r - 1));

    return T_row(r, k);
}
