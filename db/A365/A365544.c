/*
 * A365544  Number of subsets of {1..n} containing two distinct elements
 *          summing to n.
 * https://oeis.org/A365544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - 3^floor((n-1)/2) * 2^(n%2==0 ? 2 : 1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts subsets of {1..n} that contain two distinct elements summing to n. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */
#include "../utils/pow_seq.h" /* provides seq_t pow_seq(seq_t base, int exp) */

seq_t a(idx_t n)
{
    /* n < 3 => no subset can contain a pair summing to n */
    if (n < 3)
        return (seq_t)0;

    /* number of unordered pairs (k, n-k) with k < n-k */
    idx_t m = (n - 1) / 2;

    /* free elements not involved in any such pair:
       - if n is odd: only element n itself
       - if n is even: elements n/2 and n
     */
    int f = (n & 1ULL) ? 1 : 2;      /* f = 1 for odd n, 2 for even n */

    /* 2^n  */
    seq_t total = pow2(n);

    /* 3^m  (each pair can contribute 0,1, or 2 -> 3 choices)  */
    seq_t pair_choices = pow_seq((seq_t)3, (int)m);

    /* 2^f for the free elements */
    seq_t free_choices = pow2((idx_t)f);

    seq_t avoid = pair_choices * free_choices;   /* subsets avoiding any sum pair */

    return total - avoid;                        /* subsets that *do* contain such a pair */
}
