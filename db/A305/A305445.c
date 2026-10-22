/*
 * A305445  Minimum number of bit inversions to convert n into a prime.
 * https://oeis.org/A305445
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   minimal number of bit inversions to convert n into a prime (by
 *            flipping any bits of the binary representation of n; leading
 *            zeros in the resulting representation are allowed)
 * Range:     u8 n <= 361, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;

    /* If n is already prime, zero inversions are required. */
    if (is_prime_big(sn))
        return 0;

    /* Length of the binary representation of n. */
    idx_t len = floor_log2_uint64(n) + 1;

    /* For weight w = 0 already handled; try increasing Hamming weights. */
    for (idx_t w = 1; w <= len; ++w) {
        /* Skip unnecessary cases when w > len. */
        if (w > len)
            break;

        /* Initial mask with w set bits (lowest positions). */
        uint64_t mask = ((uint64_t)1 << w) - 1;
        /* Upper bound for masks: all w‑bit subsets of a len‑bit word. */
        uint64_t limit = ((uint64_t)1 << len);

        while (mask < limit) {
            seq_t cand = sn ^ (seq_t)mask;
            if (cand >= 2ULL && is_prime_big(cand))
                return (seq_t)w;

            /* Gosper's hack to get next combination with same popcount. */
            uint64_t c   = mask & -mask;
            uint64_t r   = mask + c;
            mask = (((r ^ mask) >> 2) / c) | r;
        }
    }

    /* Fallback: the maximum number of inversions needed is the bit length.
       (Theoretically unreachable for n >= 2.) */
    return (seq_t)len;
}
