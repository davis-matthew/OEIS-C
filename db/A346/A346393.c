/*
 * A346393  Irregular triangle read by rows: the n-th row gives the divisors
 *          of n ending with the final digit of n.
 * https://oeis.org/A346393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 481, u16 n <= 9295, u32 n <= 9295, u64 n <= 9295,
 *            u128 n <= 9295, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*

   Formula/definition:  a(n) is the n-th element of the row‑wise
   concatenation of all divisors of m that end with the last digit of m.
   For each integer m ≥ 1, list its divisors in increasing order and
   keep only those whose decimal representation ends with the same
   final digit as m.  Concatenate the lists for m = 1, 2, 3, ….
   a(n) is the n-th element of this infinite sequence.

   The function below computes a(n) on demand, without pre‑computing a
   table.  It iterates through the rows until the cumulative count of
   qualifying divisors reaches n, then extracts the required element
   from the current row.

   The algorithm uses a small stack array to gather qualifying
   divisors in ascending order.  The search is performed in
   roughly O(N √M) time, which is more than fast enough for N up
   to about 10 000 as required.

*/

#include <stddef.h>   /* for size_t */

/* --------------------------------------------------------------------- */
/* Integer square‑root (floor) for unsigned 64‑bit values.  Maximal
   values are 2^64‑1, so 64 iterations are sufficient. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid > 0 && mid > n / mid)  /* mid*mid would overflow n */
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* --------------------------------------------------------------------- */
/* Main sequence function.  seq_t and idx_t are defined by the build
   system. */
seq_t a(idx_t N)
{
    if (N == 0)          /* out of bounds; not used but safe */
        return (seq_t)0;

    idx_t cum = 0;   /* cumulative count of qualifying divisors so far */
    idx_t m = 1;     /* current row (integer whose divisors we examine) */

    /* Temporary buffers for qualifying divisors.
       256 is ample for the expected divisor counts in the range
       of interest (≤10 000).  If m becomes very large, the
       algorithm can be replaced with a more robust scheme. */
    seq_t small[256];
    seq_t big[256];
    while (1) {
        unsigned int ld = (unsigned int)(m % 10);   /* last digit of m */
        size_t g = 0;           /* count of qualifying divisors in row m */
        size_t big_cnt = 0;

        /* Count the number of qualifying divisors first */
        idx_t sq = isqrt_u64(m);
        for (idx_t i = 1; i <= sq; ++i) {
            if (m % i == 0) {
                idx_t j = m / i;
                if ((i % 10) == ld) ++g;
                if (j != i && (j % 10) == ld) ++g;
            }
        }

        if (cum + g < N) {            /* we still need to go further */
            cum += g;
            ++m;
            continue;
        }

        /* The desired element lies in the row of m */
        /* Build the list of qualifying divisors in ascending order */
        g = 0;            /* reuse as index into small[] */
        for (idx_t i = 1; i <= sq; ++i) {
            if (m % i == 0) {
                idx_t j = m / i;
                if ((i % 10) == ld) {
                    if (g < sizeof(small) / sizeof(small[0]))
                        small[g++] = (seq_t)i;
                }
                if (j != i && (j % 10) == ld) {
                    if (big_cnt < sizeof(big) / sizeof(big[0]))
                        big[big_cnt++] = (seq_t)j;
                }
            }
        }
        /* Append large divisors in reverse order to get ascending order */
        for (size_t k = big_cnt; k > 0; --k)
            small[g++] = big[k - 1];

        size_t pos = (size_t)(N - cum - 1);   /* zero‑based position */
        return small[pos];
    }
}
