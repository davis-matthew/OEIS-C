/*
 * A319663  Irregular triangle read by rows: T(n,k) = 5^k mod 2^n, n >= 2, 0
 *          <= k <= 2^(n-2) - 1.
 * https://oeis.org/A319663
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 5^{n-1-2^{floor_log2(n-1)-1}} mod 2^{2+floor_log2(n-1)}
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* floor of log2 for 64‑bit unsigned integers */
static inline unsigned int floor_log2_u64(uint64_t x)
{
    unsigned int r = 0;
    while (x >>= 1) ++r;
    return r;
}

/* modular exponentiation where base, exp, mod are all seq_t */
static inline seq_t modpow_seq(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* indices start at n = 2 */
    unsigned int r = 2 + floor_log2_u64(n - 1);
    uint64_t start_idx = 1ULL << (r - 2);          /* start index of row r */
    seq_t k = (seq_t)(n - (1 + start_idx));        /* exponent for 5^k */
    seq_t mod = pow2(r);                           /* modulus 2^r */
    return modpow_seq((seq_t)5, k, mod);           /* a(n) = 5^k mod 2^r */
}
