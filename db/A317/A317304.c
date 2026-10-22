/*
 * A317304  Numbers k with the property that both Dyck paths of the symmetric
 *          representation of sigma(k) have a central valley.
 * https://oeis.org/A317304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 4 + (i-1)*(2*i+3) + (n - (i-1)*i - 1), where i is the
 *            smallest integer with i*(i+1) ≥ n. The sequence is obtained by
 *            concatenating consecutive blocks of even length: 2,4,6,8,
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 for the OEIS sequence */
    idx_t i;

    /* Find i such that i*(i+1) >= n using an exponential search
       followed by binary search.  All interim products are
       computed in 128‑bit to avoid overflow. */
    idx_t hi = 1;
    while ((__uint128_t)hi * (hi + 1) < n)
        hi <<= 1;                /* double hi until it is large enough */

    idx_t lo = hi >> 1;          /* previous value of hi */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((__uint128_t)mid * (mid + 1) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    i = hi;                      /* minimal i with i*(i+1) ≥ n */

    /* Compute the starting value of the block (start_i) */
    __uint128_t start = (__uint128_t)4 +
                        (__uint128_t)(i - 1) *
                        ((__uint128_t)2 * i + 3);

    /* Compute the starting index of the block */
    __uint128_t block_start_idx = (__uint128_t)(i - 1) * i + 1;

    /* Offset within the block */
    idx_t offset = n - (idx_t)block_start_idx;

    /* Final value */
    __uint128_t result = start + offset;

    return (seq_t)result;
}
