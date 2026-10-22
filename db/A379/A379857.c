/*
 * A379857  Number of values of k for which n can be written as a sum of k
 *          distinct positive squares.
 * https://oeis.org/A379857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct k such that n can be written as a sum
 *            of k distinct positive squares
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   max_square_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/max_square_count.h"

/* Return number of set bits in a 64‑bit word.  GCC/Clang provide this builtin
   and C++ standard also defines it as __builtin_popcountll. */
static unsigned popcount64(uint64_t w) { return __builtin_popcountll(w); }

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0.  */
    if (n == 0) return (seq_t)1;

    /* Determine the maximum possible value of k for this n.  */
    size_t max_k = max_square_count(n);           /* <= floor((3n)^(1/3)) */
    size_t bits_per_word = 64;
    size_t words = (max_k + bits_per_word) / bits_per_word;  /* bitset width */

    /* Allocate DP table: (n+1) sums, each holding a bitset of length words. */
    size_t table_size = (size_t)(n + 1) * words;
    uint64_t *dp = (uint64_t *)calloc(table_size, sizeof(uint64_t));
    if (!dp) return (seq_t)0;   /* allocation failure -> return 0 */

    /* sum 0 is reachable with k = 0, i.e. set bit 0. */
    dp[0] = 1ULL;

    /* Iterate over all squares v = i^2 that are <= n. */
    for (size_t i = 1; (uint64_t)i * (uint64_t)i <= n; ++i) {
        uint64_t v = (uint64_t)i * (uint64_t)i;
        for (idx_t s = n; s >= (idx_t)v; --s) {
            uint64_t *src = dp + (size_t)(s - v) * words;
            uint64_t *dst = dp + (size_t)s * words;

            uint64_t carry = 0;
            for (size_t w = 0; w < words; ++w) {
                uint64_t shifted = src[w] << 1;
                /* include carry from the previous word (shifted left by 1). */
                shifted |= carry;
                carry = src[w] >> (bits_per_word - 1);
                dst[w] |= shifted;
            }
            /* Any bits beyond max_k are automatically discarded because
               the bitset size words was chosen to hold them. */
        }
    }

    /* Count the number of distinct k values possible for sum n. */
    uint64_t *final_bits = dp + (size_t)n * words;
    unsigned count = 0;
    for (size_t w = 0; w < words; ++w) count += popcount64(final_bits[w]);

    free(dp);
    return (seq_t)count;
}
