/*
 * A376165  a(n) is the least multiple of n that contains exactly one more 1
 *          in its decimal expansion than n does.
 * https://oeis.org/A376165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = least multiple of n whose decimal expansion contains
 *            exactly one more digit '1' than n. This is implemented by
 *            brute‑force search of successive multiples, counting the ones
 *            each time
 * Range:     u8 n <= 10, u16 n <= 109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_frequencies.h"   /* for digit_frequencies() */

static inline unsigned int count_ones(seq_t x) {
    unsigned int freq[10];
    digit_frequencies(x, freq);
    return freq[1];
}

seq_t a(idx_t n) {
    /* n corresponds to the OEIS offset (n >= 1). */
    if (n == 0)
        return (seq_t)0;   /* defensive, though offset is 1 */

    /* Count ones in the index itself to get the required target. */
    unsigned int target = count_ones((seq_t)n) + 1;

    /* Start with the first multiple, i.e. k = 1. */
    seq_t cand = (seq_t)n;
    for (idx_t k = 1;; ++k) {
        if (count_ones(cand) == target)
            return cand;          /* found the least qualifying multiple */
        cand += (seq_t)n;          /* next multiple: cand = (k+1)*n */
    }
}
