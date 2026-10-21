/*
 * A263018  If n is the i-th positive integer with binary weight j, then a(n)
 *          is the j-th positive integer with binary weight i.
 * https://oeis.org/A263018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 97, u64 n <= 211,
 *            u128 n <= 522, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A263018
 *
 * Definition:
 *  For a positive integer n, let j be the binary weight (population count) of n.
 *  Let i be the 1‑based index of n among all positive integers with binary weight j.
 *  Then a(n) is the j‑th number among all positive integers with binary weight i.
 *
 *  This code computes a(n) by:
 *   1. Determining j = popcount(n).
 *   2. Ranking n among weight‑j numbers (k = i).
 *   3. Unranking: finding the k‑th number of weight i (which has weight j).
 *
 * Complexity:
 *  O(log n) per call (max ~64 iterations).
 */

#include "../utils/binom.h"          /* returns seq_t binom(idx_t a, idx_t b) */

static seq_t rank_weight(idx_t n, unsigned int weight)
/* Rank n among all positive integers with binary weight `weight`
 * 1‑based index. */
{
    seq_t rank = (seq_t)1;                /* start at 1 (counts n itself) */
    unsigned int ones_needed = weight;
    idx_t msb_pos = 0;

    /* find most significant bit position of n */
    while (n >> (msb_pos + 1)) ++msb_pos;

    idx_t remaining_bits = msb_pos;      /* bits lower than current position */

    for (int bit = (int)msb_pos; bit >= 0; --bit) {
        if (n & (((idx_t)1) << bit)) {     /* current bit is 1 */
            if (ones_needed <= remaining_bits) {
                rank += binom(remaining_bits, ones_needed);
            }
            --ones_needed;                /* consume this 1 */
        }
        --remaining_bits;                  /* move to the next lower bit */
    }
    return rank;
}

static seq_t kth_of_weight(unsigned int weight, seq_t kth)
/* Return the `kth` (1‑based) positive integer having binary weight `weight`. */
{
    if (weight == 0) return (seq_t)0;          /* not used for this sequence */

    seq_t cumulative = (seq_t)0;
    idx_t L = weight;                          /* minimal possible length */

    /* Find minimal length L with cumulative + count >= kth. */
    while (1) {
        seq_t count = binom(L - 1, weight - 1); /* numbers of weight `weight`
                                                  with highest bit at position L-1 */
        if (cumulative + count >= kth) break;
        cumulative += count;
        ++L;
    }

    seq_t k_remain = kth - cumulative;         /* rank within numbers of length L */
    seq_t result = ((seq_t)1) << (L - 1);       /* set the leading 1 */
    unsigned int ones_needed = weight - 1;      /* remaining ones to place */

    for (int pos = (int)(L - 2); pos >= 0 && ones_needed > 0; --pos) {
        seq_t cnt = binom(pos, ones_needed);   /* count if this bit is 0 */
        if (cnt >= k_remain) {
            /* keep bit 0 */
        } else {
            /* set bit 1 */
            result |= ((seq_t)1) << pos;
            k_remain -= cnt;
            --ones_needed;
        }
    }
    return result;
}

seq_t a(idx_t n)
/*OEIS A263018: a(n) as defined above.
 *  n ≥ 1 (offset 1). */
{
    /* 1. binary weight of n */
    unsigned int j = __builtin_popcountll((unsigned long long)n);

    /* 2. rank of n among weight‑j numbers → i */
    seq_t i = rank_weight(n, j);

    /* 3. find the j‑th number of weight i */
    return kth_of_weight((unsigned int)i, (seq_t)j);
}
