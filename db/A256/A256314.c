/*
 * A256314  Number of partitions of 3n into exactly 5 parts.
 * https://oeis.org/A256314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of 3n into exactly 5 parts. * Uses
 *            the identity p5(N) = p_{≤5}(N-5), where p_{≤5} is the number *
 *            of partitions of N-5 into at most 5 parts. The latter is
 *            obtained * by convolving the generating function
 *            1/((1-x)(1-x^2)(1-x^3)(1-x^4)(1-x^5)) * up to the needed
 *            degree....
 * Range:     u8 n <= 9, u16 n <= 38, u32 n <= 624, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;      /* dp[i] = partitions of i into at most 5 parts */
static idx_t dp_max = 0;      /* current maximum degree of dp */

static void recompute(idx_t max_deg)
{
    seq_t *newdp = (seq_t*)malloc((max_deg + 1) * sizeof(seq_t));
    if (!newdp) exit(1);
    for (idx_t i = 0; i <= max_deg; ++i) newdp[i] = 0;
    newdp[0] = 1;
    for (idx_t part = 1; part <= 5; ++part)
        for (idx_t s = part; s <= max_deg; ++s)
            newdp[s] += newdp[s - part];
    if (dp) free(dp);
    dp = newdp;
    dp_max = max_deg;
}

seq_t a(idx_t n)
{
    /* compute number for 3n */
    idx_t total = 3 * n;
    if (total < 5) return 0;              /* 3n less than 5 ⇒ no partitions */

    idx_t target = total - 5;             /* coefficient index in dp */
    if (target > dp_max) recompute(target);
    return dp[target];
}
