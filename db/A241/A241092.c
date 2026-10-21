/*
 * A241092  Number of partitions p of n into distinct parts such that max(p) =
 *          1 + 2*(number of parts of p).
 * https://oeis.org/A241092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of partitions p of n into distinct parts such that max(p)
 *            = 1 + 2*(number of parts of p)
 * Range:     u8 n <= 59, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoized: it computes the sequence up to the
   largest n requested and stores the values in a static array. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static idx_t _cache_size = 0;
static seq_t * _cache = NULL;

/* Compute the sequence up to N (inclusive) and store it in _cache. */
static void compute_sequence_up_to(idx_t N)
{
    /* Find the largest odd m such that a partition using that m can
       possibly sum to at most N.  The minimal sum for given m is
       m + (m-3)(m-1)/8.  (Derived from m = 1+2k, k=(m-1)/2).   */
    idx_t Mmax = 1;
    for (idx_t m = 1; m <= N; m += 2)
    {
        seq_t minSum = (seq_t)m + (((seq_t)(m - 3) * (seq_t)(m - 1)) / 8);
        if (minSum <= N)
            Mmax = m;
        else
            break;   /* minSum increases with m */
    }

    /* Number of parts kmax for the largest admissible m. */
    idx_t kmax = 0;
    if (Mmax >= 3)
        kmax = (Mmax - 1) / 2;

    /* Allocate auxiliary structures: dp and a(n). */
    int N_int = (int)N;
    int kmax_int = (int)kmax;

    /* dp[size][sum] : subsets of distinct numbers up to current i
       having exact size 'size' and sum 'sum'.  Size ranges 0..kmax. */
    size_t dp_rows = kmax_int + 1;
    seq_t *dp = (seq_t *)calloc(dp_rows * (N_int + 1), sizeof(seq_t));
    dp[0 * (N_int + 1) + 0] = 1;          /* empty subset */

    seq_t *a = (seq_t *)calloc(N_int + 1, sizeof(seq_t));   /* result array */

    /* Main loop over potential maximal part values m = i. */
    for (idx_t i = 1; i <= Mmax; ++i)
    {
        int i_int = (int)i;

        /* If i is an odd admissible maximal part, add contributions. */
        if ((i & 1) && i >= 3)
        {
            int k = (i_int - 1) / 2;          /* number of parts */
            int need_size = k - 1;            /* size of the remaining subset */

            for (int n = i_int; n <= N_int; ++n)
            {
                int rem = n - i_int;
                a[n] += dp[need_size * (N_int + 1) + rem];
            }
        }

        /* Update dp with the current element i, except for the last
           admissible maximal part Mmax (since no larger m will be used). */
        if (i < Mmax)
        {
            int maxSize = (i_int < kmax_int) ? i_int : kmax_int;
            for (int sz = maxSize; sz >= 1; --sz)
            {
                seq_t *dpSize = dp + sz * (N_int + 1);
                seq_t *dpPrev = dp + (sz - 1) * (N_int + 1);
                for (int s = N_int; s >= i_int; --s)
                {
                    dpSize[s] += dpPrev[s - i_int];
                }
            }
        }
    }

    /* Copy result into the global cache. */
    for (int i = 0; i <= N_int; ++i)
        _cache[i] = a[i];

    free(dp);
    free(a);
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (_cache_size <= n)
    {
        free(_cache);
        _cache_size = n + 1;
        _cache     = (seq_t *)calloc(_cache_size, sizeof(seq_t));
        compute_sequence_up_to(n);
    }
    return _cache[n];
}
