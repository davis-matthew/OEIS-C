/*
 * A321906  Irregular table read by rows: T(n,k) is the smallest m such that
 *          m^(-1/m) == 2*k + 1 (mod 2^n), 0 <= k <= 2^(n-1) - 1.
 * https://oeis.org/A321906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where r = floor(log2 n)+1, k = n - 2^(r-1). T(r,k)
 *            is the smallest odd m < 2^r such that m * (2*k+1)^m ≡ 1 (mod
 *            2^r)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"  /* provides: seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

static unsigned int floor_log2_idx(idx_t x) {
    /* x >= 1 */
    return 63 - __builtin_clzll((unsigned long long)x);
}

seq_t a(idx_t n) {
    /* row number r (>=1) */
    unsigned r = floor_log2_idx(n) + 1;
    /* start index of the row */
    idx_t row_start = ((idx_t)1) << (r - 1);
    /* column k (0‑based) */
    idx_t k = n - row_start;
    /* modulus 2^r */
    seq_t mod = ((seq_t)1) << r;
    /* odd number a = 2*k+1 */
    seq_t a_val = ((seq_t)2 * k) + 1;

    /* brute force search for the smallest odd m satisfying the congruence */
    for (uint64_t m = 1; m < (uint64_t)mod; m += 2) {
        seq_t pow_val = mod_pow(a_val, m, mod);
        if ( (((seq_t)m) * pow_val) % mod == 1 )
            return (seq_t)m;
    }
    /* Should never reach here */
    return 1;
}
