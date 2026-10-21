/*
 * A253515  Count down from 2*k to 1, then from 2*(k+1) to 1 and so on.
 * https://oeis.org/A253515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - n + floor(sqrt(n)+3/2) * floor(sqrt(n)+1/2). Computed
 *            by finding the block k such that k*(k+1) ≥ n, then returning 2*k
 *            minus the offset within that block
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* <stdint.h> may be available; otherwise basic integer types are
   assumed to be defined as part of the build environment. */
#include <stdint.h>

/* External index and value types provided by the build system. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n == 0 return 0
       (undefined value, but safe placeholder). */
    if (n == 0)
        return 0;

    /* Binary search for the smallest k with k*(k+1) ≥ n.
       Use 128‑bit arithmetic for the product to avoid overflow
       when n is very large (up to 2^64‑1). */
    idx_t low = 1;
    idx_t high = n;

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;   /* k is the block number */

    /* Length of blocks before block k: (k-1)*k.  This product
       fits in 64 bits because it is ≤ n < 2^64. */
    idx_t prev_len = (k - 1) * k;

    /* 0‑based position inside block k. */
    idx_t offset = n - prev_len - 1;

    /* Value inside the block: start at 2*k and count down. */
    seq_t val = (seq_t)(2 * k) - (seq_t)offset;
    return val;
}
