/*
 * A343673  a(0) = 1; a(n) = 3 * n * a(n-1) + Sum_{k=0..n-1} binomial(n,k) *
 *          a(k).
 * https://oeis.org/A343673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 3*n*a(n-1) + Σ_{k=0..n-1} binomial(n,k)*a(k) (n
 *            ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   grow_seq, idx_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* size_t */
#include "../utils/grow_seq.h"       /* void grow_seq(seq_t **arr, size_t *cap, size_t needed) */
#include "../utils/idx_to_seq.h"

static seq_t *a_cache = NULL;      /* cached values of a(n)   */
static size_t a_cap   = 0;          /* allocated capacity      */
static size_t a_len   = 0;          /* number of cached entries */

seq_t a(idx_t n)
{
    /* Initialise cache with a(0) if empty */
    if (a_len == 0) {
        if (a_cap == 0) grow_seq(&a_cache, &a_cap, 2);   /* capacity for 0 and 1 at least */
        a_cache[0] = (seq_t)1;
        a_len = 1;
    }

    /* Compute missing terms up to n */
    if (a_len <= n) {
        for (idx_t curr = a_len; curr <= n; ++curr) {
            /* Σ_{k=0..curr-1} binomial(curr,k) * a(k) */
            seq_t sum = 0;
            seq_t bin = (seq_t)1;          /* binomial(curr,0) */
            for (idx_t k = 0; k < curr; ++k) {
                sum += bin * a_cache[k];
                if (k + 1 < curr) {
                    /* binomial(curr,k+1) = binomial(curr,k) * (curr-k) / (k+1) */
                    bin = (bin * idx_to_seq(curr - k)) / idx_to_seq(k + 1);
                }
            }

            /* Recurrence part: 3 * curr * a(curr-1) */
            seq_t term = (seq_t)3 * idx_to_seq(curr) * a_cache[curr - 1];

            seq_t a_curr = term + sum;

            /* Ensure capacity and store result */
            if (a_cap <= (size_t)curr) {
                grow_seq(&a_cache, &a_cap, (size_t)curr + 1);
            }
            a_cache[curr] = a_curr;
        }
        a_len = (size_t)n + 1;
    }

    return a_cache[n];
}
