/*
 * A268358  Number of n-digit numbers in base ten having at least five
 *          different digits with no leading zeros allowed.
 * https://oeis.org/A268358
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*10^(n-1) - 189*4^n + 648*3^n - 1701*2^(n-1) + 504
 *            (valid for all n >= 5; returns 0 for n < 5)
 * Range:     u8 none, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 969 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_u64_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;

    seq_t res;

    /* 9*10^(n-1) */
    res = mul_small(pow10(n - 1), 9);

    /* -189*4^n */
    {
        seq_t tmp = mul_small(ipow_u64_idx(4, n), 189);
        res -= tmp;
    }

    /* +648*3^n */
    {
        seq_t tmp = mul_small(ipow_u64_idx(3, n), 648);
        res += tmp;
    }

    /* -1701*2^(n-1) */
    {
        seq_t tmp = mul_small(ipow_u64_idx(2, n - 1), 1701);
        res -= tmp;
    }

    /* +504 */
    res += 504;

    return res;
}
