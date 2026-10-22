/*
 * A365527  a(n) = Sum_{k=0..floor((n-2)/4)} Stirling2(n,4*k+2).
 * https://oeis.org/A365527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-2)/4)} Stirling2(n,4*k+2)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool */
#include <stddef.h>    /* for size_t */

/* pre‑allocate a modest amount of space (enough for the
   benchmarked range of n, which is about 10 000). */
#define MAX_N 20000

static seq_t a_cache[MAX_N+1];
static seq_t Srow0[MAX_N+1];
static seq_t Srow1[MAX_N+1];

static idx_t  cur_n     = 0;
static bool   initialized = false;
static seq_t *Sprev_ptr = Srow0;
static seq_t *Snext_ptr = Srow1;

seq_t a(idx_t n)
{
    /* Ensure that the helper tables are initialized once. */
    if (!initialized) {
        initialized = true;
        /* S(0,0) = 1 ; all other entries are zero due to static
           zero‑initialisation of the arrays. */
        Sprev_ptr[0] = (seq_t)1;
        a_cache[0]   = (seq_t)0;   /* a(0) = 0 */
    }

    /* If the requested index has already been computed, return it. */
    if (n < cur_n) {
        return a_cache[n];
    }

    /* Cap the target to the maximum storage we have.  The problem
       statement guarantees that the intended range is far below
       MAX_N, so we simply abort gracefully if it is not. */
    if (n > MAX_N) {
        /* Unsupported index; return 0 as a safe default. */
        return (seq_t)0;
    }

    /* Compute rows up to and including row n. */
    for (idx_t m = cur_n + 1; m <= n; ++m) {
        /* Compute Stirling numbers for row m in Snext_ptr. */
        Snext_ptr[0] = (seq_t)0;          /* S(m,0) = 0 for m>0 */
        for (idx_t j = 1; j <= m; ++j) {
            seq_t prev_j = (j <= cur_n) ? Sprev_ptr[j] : (seq_t)0;
            Snext_ptr[j] = Sprev_ptr[j-1] + (seq_t)j * prev_j;
        }

        /* a(m) = sum_{j%4==2} S(m,j) */
        seq_t sum_a = (seq_t)0;
        for (idx_t j = 2; j <= m; j += 4) {
            sum_a += Snext_ptr[j];
        }
        a_cache[m] = sum_a;

        /* Prepare for the next iteration: the new row becomes the
           current row.  Swap the pointers instead of copying arrays. */
        seq_t *tmp = Sprev_ptr;
        Sprev_ptr = Snext_ptr;
        Snext_ptr = tmp;

        cur_n = m;
    }

    return a_cache[n];
}
