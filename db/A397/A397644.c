/*
 * A397644  a(n) is the number of subsequences divisible by 3 taken from the
 *          n-th number whose digits are in nondecreasing order and all digits
 *          <= 3.
 * https://oeis.org/A397644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 217, u16 n <= 1137, u32 n <= 7137, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A397644: number of subsequences divisible by 3 taken from
   the n‑th number whose digits are nondecreasing and all <=3.
   Algorithm:
   1. Determine the length L of the n‑th number by accumulating
      C(L+2,2)  (numbers of length L).
   2. Find the k‑th number of length L (1‑based) and from it extract
      the counts (c1,c2,c3) of digits 1,2,3 by combinatorial
      enumeration.
   3. Count subsequences divisible by 3:
         Sum_{t1=0..c1}\sum_{t2=0..c2} [ (t1+2t2)%3==0 ] *
            binomial(c1,t1)*binomial(c2,t2)
      Multiply the sum by 2^c3 (subsets of the digit '3' give residue 0)
      and subtract 1 to remove the empty subsequence.  */

/* required helper functions */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */
#include "../utils/pow2.h"       /* pow2(idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe guard */

    idx_t L = 1;
    seq_t cumulative = 0;

    /* find smallest L with cumulative >= n */
    for (;;) {
        /* count of numbers of length L: C(L+2,2) */
        seq_t inc = binomial((idx_t)(L + 2), 2);
        cumulative += inc;
        if (cumulative >= (seq_t)n) break;
        ++L;
    }

    /* index within length L numbers (1‑based) */
    seq_t inc = binomial((idx_t)(L + 2), 2);
    seq_t prev_cum = cumulative - inc;
    idx_t k = (idx_t)((seq_t)n - prev_cum);   /* 1‑based in this block */

    /* determine digit counts (c1,c2,c3) */
    idx_t c1, c2, c3;
    idx_t rem = k;
    for (c1 = L; ; --c1) {                     /* iterate c1 from L downto 0 */
        idx_t cnt = (L - c1) + 1;               /* possible c2 values */
        if (rem > cnt) {
            rem -= cnt;
        } else {
            break;  /* found c1 */
        }
    }
    c2 = (L - c1) - (rem - 1);
    c3 = L - c1 - c2;

    /* count subsequences divisible by 3 */
    seq_t total = 0;
    for (idx_t t1 = 0; t1 <= c1; ++t1) {
        seq_t ways1 = binomial(c1, t1);
        for (idx_t t2 = 0; t2 <= c2; ++t2) {
            if ( (t1 + 2 * t2) % 3 != 0 ) continue;
            seq_t ways2 = binomial(c2, t2);
            total += ways1 * ways2;
        }
    }

    /* incorporate all subsets of the '3' digits */
    seq_t pow2c3 = pow2(c3);
    total *= pow2c3;

    /* exclude empty subsequence if any non‑empty counted */
    if (total > 0) total -= 1;

    return total;
}
