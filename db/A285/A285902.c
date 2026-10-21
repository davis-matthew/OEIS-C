/*
 * A285902  Total number of partitions of all positive integers <= n into an
 *          even number of consecutive parts.
 * https://oeis.org/A285902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A285902
   a(n) = Total number of partitions of all positive integers ≤ n into
          an even number of consecutive parts. */

/* The function enumerates all even‑length consecutive partitions for
   each m ≤ n, accumulates their counts for each k, and then builds a
   prefix sum array.  Subsequent calls with larger n extend the
   computation incrementally. */

#include <stdlib.h>

/* static state -------------------------------------------------------------- */
static idx_t max_computed = 0;   /* largest n that has been processed */
static seq_t *rep = NULL;         /* number of representations for each k */
static seq_t *cum = NULL;         /* prefix sums of rep */

/* helper: recompute data up to index n (inclusive) -------------------------- */
static void recompute(idx_t n)
{
    if (max_computed >= n)
        return;                  /* already have data up to n */

    /* free any previous buffer (the memory will be reallocated below) */
    if (rep) free(rep);
    if (cum) free(cum);

    /* allocate arrays of length n+1 (indices 0..n) */
    rep = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    cum = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!rep || !cum)              /* allocation failure → further calls will
                                       simply return 0 */
        return;

    /* bignumber for comparison against n */
    seq_t N = (seq_t)n;

    /* count representations with an even number of consecutive parts */
    for (idx_t m = 2; m <= n; m += 2)      /* m = length, even */
    {
        /* first sum of m consecutive numbers starting from 1:
           S_0 = m*(m-1)/2 */
        seq_t base = ((seq_t)m * ((seq_t)m - 1)) / 2;
        if (base > N) continue;            /* no such representation for this m */

        for (idx_t a = 1;; ++a)            /* a = starting integer */
        {
            /* S = a*m + base  */
            seq_t sum = base + (seq_t)m * (seq_t)a;
            if (sum > N)
                break;
            rep[(idx_t)sum] += (seq_t)1;
        }
    }

    /* build prefix sums: cum[k] = ∑_{i=1..k} rep[i] */
    cum[0] = 0;
    for (idx_t k = 1; k <= n; ++k)
        cum[k] = cum[k-1] + rep[k];

    max_computed = n;
}

/* public function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    recompute(n);
    return cum[n];
}
