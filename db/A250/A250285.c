/*
 * A250285  Number of permutations p of [n] such that p(i) > p(i+1) iff i == 0
 *          (mod 8).
 * https://oeis.org/A250285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of permutations of [n] in which p[i] > p[i+1]
 *            iff i ≡ 0 (mod 8). The computation is performed by dynamic
 *            programming over the blocks of length 8 separated by these
 *            required descents
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 23, u64 n <= 33, u128 n <= 51,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm works for all n ≥ 0.  For every n it first
   determines the block sizes (8 for every full block, the
   remaining r = n mod 8 for the last block – r = 8 if n is a multiple
   of 8).  Then it builds two arrays dp_even and dp_odd, where
   dp_even[i] (resp. dp_odd[i]) stores the sum of 1 / (∏ group_factorial)
   over all ways to partition the first i blocks into an even (resp.
   odd) number of continuous groups.  Adding a new group always flips
   the parity, so the recurrence is

       dp_even[i] += dp_odd[j] * C(tot_j + g, g)
       dp_odd[i]  += dp_even[j] * C(tot_j + g, g)

   for every j < i, where g is the size of the new group and
   tot_j is the total size of the first j blocks.
   The final answer is
       a(n) = (–1)^B * (dp_even[B] – dp_odd[B]),
   where B is the number of blocks.  Because the expression inside
   is non‑negative for all B, the subtraction can be performed
   safely on unsigned integers.  All arithmetic uses seq_t,
   which may be an unsigned integer type or an arbitrary‑precision
   integer class. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* n = 0: only the empty permutation */
    if (n == 0)
        return (seq_t)1;

    /* Determine the block sizes */
    idx_t m = (n - 1) / 8;        /* number of full size–8 blocks before the last one */
    idx_t r = n - m * 8;          /* size of the last block (1 … 8, 8 if n is a multiple of 8) */
    idx_t B = m + 1;              /* total number of blocks */

    idx_t *s = (idx_t *)malloc(B * sizeof(idx_t));
    for (idx_t i = 0; i < m; ++i)
        s[i] = 8;
    s[m] = r;

    /* Prefix sums of block sizes – pref[i] = total size of first i blocks */
    idx_t *pref = (idx_t *)malloc((B + 1) * sizeof(idx_t));
    pref[0] = 0;
    for (idx_t i = 0; i < B; ++i)
        pref[i + 1] = pref[i] + s[i];

    /* Dynamic programming arrays for even/odd parity of group count */
    seq_t *dp_even = (seq_t *)malloc((B + 1) * sizeof(seq_t));
    seq_t *dp_odd  = (seq_t *)malloc((B + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= B; ++i) {
        dp_even[i] = 0;
        dp_odd[i]  = 0;
    }
    dp_even[0] = (seq_t)1;  /* zero groups – even parity */

    for (idx_t i = 1; i <= B; ++i) {
        seq_t val_even = 0;
        seq_t val_odd  = 0;
        for (idx_t j = 0; j < i; ++j) {
            idx_t group_size = pref[i] - pref[j];
            idx_t tot_j      = pref[j];
            seq_t bin = binom(tot_j + group_size, group_size);
            if (dp_odd[j] != 0) {
                val_even += dp_odd[j] * bin;   /* from odd to even */
            }
            if (dp_even[j] != 0) {
                val_odd += dp_even[j] * bin;   /* from even to odd */
            }
        }
        dp_even[i] = val_even;
        dp_odd[i]  = val_odd;
    }

    seq_t result;
    if ((B & 1) == 0) {
        /* B even: a(n) = dp_even[B] – dp_odd[B] */
        result = dp_even[B];
        result -= dp_odd[B];
    } else {
        /* B odd:  a(n) = dp_odd[B] – dp_even[B] */
        result = dp_odd[B];
        result -= dp_even[B];
    }

    free(s);
    free(pref);
    free(dp_even);
    free(dp_odd);
    return result;
}
