/*
 * A340755  Number of partitions of n into 4 parts such that the sum of the
 *          smallest two parts and the sum of the largest two parts are both
 *          squarefree.
 * https://oeis.org/A340755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..⌊n/4⌋} Σ_{j=k..⌊(n−k)/3⌋} Σ_{i=j..⌊(n−j−k)/2⌋}
 *            [μ(k+j)^2]·[μ(n−k−j)^2] where μ(m)^2 is 1 iff m is square‑free,
 *            else 0
 * Range:     u8 n <= 39, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation precomputes an array of square‑free indicators up
   to the maximum n seen so far and then evaluates the triple sum
   directly.  Complexity is Θ(n²) and the memory footprint is
   Θ(n).  It works for any seq_t/index_t types used by the build
   environment. */

#include <stdlib.h>
#include <stdbool.h>

/* forward declaration: seq_t and idx_t are provided by the build. */
seq_t a(idx_t n);

/* --- helper: precompute square‑free array ---------------------------------- */
static uint8_t *sqfree_arr = NULL;
static idx_t   sqfree_max  = 0;

static void
init_sqfree(idx_t limit)
{
    if (limit <= sqfree_max) return;          /* already ready */

    /* allocate new array and initialise to true (square‑free) */
    idx_t new_size = limit + 1;
    uint8_t *new_arr = (uint8_t *)malloc((size_t)new_size);
    if (!new_arr) return;                     /* let caller fail if out of memory */
    for (idx_t i = 0; i < new_size; ++i) new_arr[i] = 1;
    new_arr[0] = 0;                           /* 0 is not used, but mark as false */

    /* mark every composite square p^2 as non‑square‑free */
    for (idx_t p = 2; p * p <= limit; ++p) {
        idx_t sq = p * p;
        for (idx_t j = sq; j <= limit; j += sq) new_arr[j] = 0;
    }

    /* replace the old array */
    free(sqfree_arr);
    sqfree_arr = new_arr;
    sqfree_max = limit;
}

/* --- main function ------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* if n is very small no partitions exist */
    if (n < 4) return 0;

    /* initialise square‑free array up to n */
    init_sqfree(n);
    if (!sqfree_arr) return 0; /* malloc failure */

    seq_t sum = 0;

    /* outer loop over the smallest part k */
    for (idx_t k = 1; k <= n / 4; ++k) {
        idx_t maxj = (n - k) / 3;
        if (maxj < k) continue;          /* no valid j */

        /* middle loop over the second part j */
        for (idx_t j = k; j <= maxj; ++j) {
            idx_t t = n - k - j;          /* remaining sum for i + d  */
            idx_t maxi = t / 2;           /* because i <= d => 2*i <= t */
            if (maxi < j) continue;      /* no valid i for this (k,j) */

            seq_t cnt_i = (seq_t)(maxi - j + 1);  /* number of admissible i's */

            /* both sums must be square‑free */
            if (!sqfree_arr[k + j]) continue;     /* small two parts sum not square‑free */
            if (!sqfree_arr[t])    continue;     /* large two parts sum not square‑free */

            sum += cnt_i;
        }
    }

    return sum;
}
