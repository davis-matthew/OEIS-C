/*
 * A333257  Number of distinct consecutive subsequence-sums of the k-th
 *          composition in standard order.
 * https://oeis.org/A333257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A333257: Number of distinct consecutive subsequence‐sums of the k‑th
 * composition in standard order.
 * The algorithm follows the description in the OEIS entry: for a non‑zero
 * index k, write k in binary, take the positions of the 1 bits (counted
 * from 1 at the least significant bit), prepend 0, form successive
 * differences, then reverse the differences to get the composition.
 * We then enumerate all subarray sums of this small composition and
 * count the distinct values (including 0).
 */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Composition of n = 0 is empty; only sum 0 is possible. */
    if (n == 0)
        return (seq_t)1;

    /* Build the composition parts.  The number of parts equals the
       number of set bits in n.  The sum of all parts is at most
       floor_log2(n)+1 <= 64. */
    uint32_t parts[64];
    size_t parts_len = 0;

    idx_t tmp = n;
    idx_t pos   = 1;     /* position of the current bit (1‑based) */
    idx_t prev  = 0;     /* previous position of a 1‑bit */

    while (tmp != 0) {
        if (tmp & 1ULL) {
            parts[parts_len++] = (uint32_t)(pos - prev);
            prev = pos;
        }
        pos++;
        tmp >>= 1;
    }

    /* Reverse the parts array to obtain the standard‑order composition. */
    for (size_t i = 0; i < parts_len / 2; ++i) {
        uint32_t t = parts[i];
        parts[i] = parts[parts_len - 1 - i];
        parts[parts_len - 1 - i] = t;
    }

    /* Count distinct sums of all contiguous subsequences, including the
       empty subsequence whose sum is 0.  The total sum of the composition
       is at most 64, so a 2×64 bit bitset suffices. */
    uint64_t seen[2] = { 1ULL /* sum 0 */ , 0ULL };

    for (size_t i = 0; i < parts_len; ++i) {
        uint32_t sum = 0;
        for (size_t j = i; j < parts_len; ++j) {
            sum += parts[j];
            seen[sum >> 6] |= 1ULL << (sum & 63);
        }
    }

    /* Count the set bits in the bitset. */
    size_t distinct = __builtin_popcountll(seen[0]) + __builtin_popcountll(seen[1]);

    return (seq_t)distinct;
}
