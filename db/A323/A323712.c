/*
 * A323712  Number of the following described shuffles required to return a
 *          deck of n cards to its original state. Create two piles by
 *          alternating a card from the top of the deck left-right-left-right
 *          until the deck is exhausted. Then, placing the left pile on top of
 *          the right pile constitutes one shuffle.
 * https://oeis.org/A323712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the order of the permutation induced by an * out‑shuffle
 *            defined by taking cards alternately into two piles * (top cards
 *            are put on top of the piles) and placing the left pile * on top
 *            of the right pile
 * Range:     u8 n <= 261, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the cycle decomposition of that
 * permutation and returns the least common multiple of the cycle
 * lengths. It works for any n ≥ 1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n = 0 */

    /* If n is small we could skip allocation, but a uniform
     * approach keeps the code simple. */
    bool *vis = (bool *)calloc((size_t)(n + 1), sizeof(bool));
    if (!vis) return 0;           /* allocation failed */

    /* Pre‑compute constants used in the permutation formula. */
    const idx_t p = (n + 1) >> 1;  /* ceil(n/2) – number of odd indices   */
    const idx_t m = n >> 1;        /* floor(n/2) – number of even indices*/

    seq_t lcm = 1;                 /* current least common multiple */

    for (idx_t i = 1; i <= n; ++i) {
        if (!vis[i]) {
            idx_t j = i;
            idx_t len = 0;
            /* follow the cycle starting at i */
            do {
                vis[j] = true;
                ++len;
                /* compute next position according to the shuffle rule */
                if (j & 1U) { /* j is odd */
                    j = p - ((j - 1) >> 1);
                } else {     /* j is even */
                    j = p + m - (j >> 1) + 1;
                }
            } while (j != i);

            /* update lcm with the cycle length */
            seq_t cycle_len = (seq_t)len;
            seq_t g = gcd(lcm, cycle_len);
            lcm = (lcm / g) * cycle_len;
        }
    }

    free(vis);
    return lcm;
}
