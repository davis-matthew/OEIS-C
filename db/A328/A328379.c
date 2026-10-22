/*
 * A328379  a(n) is the sum of the distinct numbers whose binary digits appear
 *          in order but not necessarily as consecutive digits in the binary
 *          representation of n.
 * https://oeis.org/A328379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 2345, u32 n <= 9826, u64 n <= 9826,
 *            u128 n <= 9826, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm for OEIS A328379
 *
 * a(n) is the sum of the distinct numbers whose binary digits appear
 * in order (not necessarily consecutively) in the binary representation of n.
 *
 * The algorithm iterates over the bits of n from most significant to
 * least significant and keeps track of the distinct subsequence strings
 * that start with a 1.  For each new bit it updates:
 *
 *   count1 – number of distinct subsequence strings that start with 1
 *   sum1   – sum of their integer values
 *
 * New unique strings created by the current bit are obtained by
 * appending this bit to every previously known string that starts with 1,
 * plus the singleton string '1' if the current bit is 1.
 *
 * Duplicates that would have been produced by an earlier occurrence of the
 * same bit are removed by remembering what was added during the last
 * processing of that bit.
 *
 * Complexity: O(#bits of n) time, O(1) memory.  Works for all n that fit
 * in the index type idx_t (up to 64 bits).  All arithmetic on the result
 * is performed in seq_t, as required.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* a(0) = 0 */

    /* Count of distinct subsequence strings that start with 1 */
    uint64_t count1 = 0;
    seq_t    sum1   = 0;

    /* Information about the strings added when processing the last
     * occurrence of bit 0 and 1 respectively. */
    uint64_t last_new_count[2] = {0, 0};
    seq_t    last_new_sum[2]   = {0, 0};

    /* Iterate over bits from most‑significant to least‑significant */
    int msb = floor_log2_uint64(n);
    for (int pos = msb; pos >= 0; --pos) {
        int bit = (n >> pos) & 1U;            /* current bit (0 or 1) */

        /* New strings that would be created by adding this bit */
        uint64_t new_count = count1 + (bit ? 1 : 0);
        seq_t    new_sum   = (seq_t)2 * sum1 +    /* v -> 2*v + bit */
                            (seq_t)bit * (seq_t)count1 +
                            (bit ? (seq_t)1 : (seq_t)0); /* singleton */

        /* Duplicates caused by the previous occurrence of the same bit */
        uint64_t dup_count = last_new_count[bit];
        seq_t    dup_sum   = last_new_sum[bit];

        /* Unique additions */
        uint64_t added_count = new_count - dup_count;
        seq_t    added_sum   = new_sum - dup_sum;

        /* Update totals */
        count1 += added_count;
        sum1   += added_sum;

        /* Record what was added this time for future duplicate removal */
        last_new_count[bit] = new_count;
        last_new_sum[bit]   = new_sum;
    }

    /* sum1 contains the sum of all distinct numbers that contain at least
     * one 1.  Zero is also obtainable (empty or all‑zero subsequences)
     * and contributes nothing to the sum, so we may return sum1 directly. */
    return sum1;
}
