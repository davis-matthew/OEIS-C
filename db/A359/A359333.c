/*
 * A359333  a(1) = 0, and for any n > 1, a(n) is chosen among 0 and 1 so as to
 *          minimize the length of the longest sequence of distinct integers
 *          in arithmetic progression in the interval 1..n and containing n
 *          where the sequence is constant; in case of a tie, maximize the
 *          least common difference in those longe...
 * https://oeis.org/A359333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computed by exhaustive search: for each n>1, try a(n)=0 and a(n)=1;
    for each candidate measure the longest constant arithmetic progression
    that contains n and lies entirely in 1..n.  Choose the value that
    minimises that length; if equal, choose the value whose longest
    progressions have the larger minimal common difference. */
#include <stdlib.h>
#include <stdbool.h>

/* Cache of computed sequence values (1‑based).  Initially empty. */
static seq_t *cache = NULL;      /* array holding terms 0..computed_len   */
static size_t cap   = 0;         /* allocated capacity (>= index+1)      */
static size_t computed_len = 0;  /* highest index that has a value */

/* Ensure the cache can hold indices 0..need-1.  On first call it
   allocates space for 16 entries. */
static void ensure_cache(size_t need)
{
    if (cap >= need) return;
    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cap   = new_cap;
}

/* Main sequence function.  Works for arbitrary call order, memoising
   all previously computed terms. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* unused, but silences compiler */

    if (n <= computed_len) return cache[n];

    /* Compute terms from computed_len+1 up to n. */
    for (idx_t m = computed_len + 1; m <= n; ++m)
    {
        seq_t best_val = 0;
        idx_t best_max_len = (idx_t)-1;   /* initialise with very large value */
        idx_t best_min_diff = 0;          /* minimal d among longest runs */

        for (int guess = 0; guess <= 1; ++guess)
        {
            seq_t val = (seq_t)guess;
            idx_t max_len   = 0;
            idx_t min_d_of_max = m;        /* initial – any d will be smaller */

            for (idx_t d = 1; d < m; ++d)
            {
                /* No point if the whole AP is shorter than the best found. */
                idx_t theoretical = (m - 1) / d + 1;
                if (theoretical < max_len) continue;

                /* Count consecutive equal values starting at m and stepping
                   backwards by d until a mismatch or the start of the interval. */
                idx_t run_len = 1;
                idx_t i = m - d;
                while (i >= 1 && cache[i] == val)
                {
                    ++run_len;
                    if (i < d) break;
                    i -= d;
                }

                if (run_len > max_len)
                {
                    max_len = run_len;
                    min_d_of_max = d;
                }
                else if (run_len == max_len && d < min_d_of_max)
                {
                    min_d_of_max = d;
                }
            }

            /* Choose between the two guesses.  Prefer smaller max_len,
               and if equal, prefer larger minimal difference. */
            if (max_len < best_max_len ||
               (max_len == best_max_len && min_d_of_max > best_min_diff))
            {
                best_val      = val;
                best_max_len  = max_len;
                best_min_diff = min_d_of_max;
            }
        }

        ensure_cache(m + 1);       /* make room for index m */
        cache[m] = best_val;
    }

    computed_len = n;
    return cache[n];
}
