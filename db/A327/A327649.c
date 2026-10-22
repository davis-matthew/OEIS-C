/*
 * A327649  Maximum value of powers of 2 mod n.
 * https://oeis.org/A327649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = maximum of 2^k mod n for k ≥ 0.
 * The function uses the decomposition n = 2^j · r with r odd.
 * For r = 1 (n a power of 2) a(n) = 2^(j‑1) for j > 0, a(1) = 0.
 * For r > 1, a(n) = 2^j · a(r), where a(r) is found by iterating
 * powers of 2 modulo r until the cycle repeats.
 */

#include <stdint.h>

static inline uint64_t mul2_mod(uint64_t v, uint64_t m)
{
    /* For small n (≤10 000) this multiplication cannot overflow 64‑bit */
    return (v * 2) % m;
}

seq_t a(idx_t n)
{
    if (n <= 1)        /* a(1) = 0 */
        return 0;

    /* Count trailing zeros to separate power of two factor */
    idx_t j = 0;
    idx_t tmp = n;
    while ((tmp & 1U) == 0)
    {
        tmp >>= 1;
        ++j;
    }
    /* tmp is now the odd part r */
    idx_t r = tmp;

    /* Compute 2^j in seq_t */
    seq_t pow2_j = 1;
    for (idx_t i = 0; i < j; ++i)
        pow2_j <<= 1;

    if (r == 1)        /* n is a power of two */
    {
        /* a(2^k) = 2^(k-1) for k > 0 */
        seq_t res = 1;
        for (idx_t i = 1; i < j; ++i)  /* shift j‑1 times */
            res <<= 1;
        return res;
    }

    /* r is odd > 1: compute a(r) by iterating powers of 2 modulo r */
    uint64_t val = 1;
    uint64_t max_val = 1;
    for (;;)
    {
        val = mul2_mod(val, r);
        if (val > max_val)
            max_val = val;
        if (val == 1)
            break;
    }

    /* a(n) = 2^j * a(r) */
    return pow2_j * (seq_t)max_val;
}
