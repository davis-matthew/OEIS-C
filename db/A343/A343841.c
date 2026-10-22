/*
 * A343841  a(n) = Sum{k=0..n} (-1)^(n-k)*binomial(n, k)*Stirling2(n, k).
 * https://oeis.org/A343841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum{k=0..n} (-1)^(n-k) * binomial(n,k) * Stirling2(n,k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 22, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Memoised cache for a(n) values */
static seq_t *a_cache = NULL;
static size_t a_len = 0;   /* number of values currently cached (a_len-1 is last idx) */
static size_t a_cap = 0;   /* allocated capacity of a_cache */

/* Two rows of the Stirling‑2 triangle.
   S_prev holds the last computed row, S_curr is the row under construction. */
static seq_t *S_prev = NULL, *S_curr = NULL;
static size_t S_cap = 0;   /* allocated capacity for the two rows */
static int S_initialised = 0; /* flag that S_prev[0] has been set to 1 */

/* Ensure a_cache can store values up to index n */
static void ensure_a_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed > a_cap) {
        a_cache = (seq_t*)realloc(a_cache, needed * sizeof(seq_t));
        a_cap   = needed;
    }
}

/* Ensure S_prev and S_curr can store at least n+1 entries.
   Once allocated initialise S_prev[0] to 1. */
static void ensure_S_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed > S_cap) {
        S_prev = (seq_t*)realloc(S_prev, needed * sizeof(seq_t));
        S_curr = (seq_t*)realloc(S_curr, needed * sizeof(seq_t));
        S_cap  = needed;
    }
    if (!S_initialised) {
        S_prev[0] = 1;    /* S(0,0) = 1 */
        S_initialised = 1;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1. */
    if (n == 0) {
        if (a_len == 0) {
            ensure_a_capacity(0);
            a_cache[0] = 1;
            a_len = 1;
        }
        return a_cache[0];
    }

    /* Grow caches if we need a larger n. */
    if ((size_t)n >= a_len) {
        ensure_a_capacity(n);
        ensure_S_capacity(n);

        /* Compute rows from a_len up to n inclusive. */
        for (size_t cur = a_len; cur <= (size_t)n; ++cur) {
            size_t prev_size = cur - 1;          /* size of S_prev row (prev_size+1 elems) */
            /* Make sure arrays are large enough for this row. */
            if (prev_size + 1 > S_cap) {
                S_prev = (seq_t*)realloc(S_prev, (cur + 1) * sizeof(seq_t));
                S_curr = (seq_t*)realloc(S_curr, (cur + 1) * sizeof(seq_t));
                S_cap  = cur + 1;
            }

            S_curr[0] = 0;                       /* S(cur,0) = 0 for cur>0 */

            seq_t binom = 1;     /* C(cur,0) */
            seq_t sum   = 0;     /* running sum for a(cur) */

            for (size_t k = 1; k <= cur; ++k) {
                /* Compute S(cur,k) = k*S(prev,k) + S(prev,k-1). */
                seq_t val_k   = (k <= prev_size) ? S_prev[k] : (seq_t)0;
                seq_t val_km1 = S_prev[k - 1];
                S_curr[k] = (seq_t)k * val_k + val_km1;

                /* Update binomial coefficient C(cur,k). */
                binom = binom * (seq_t)(cur - k + 1) / (seq_t)k;

                /* Sign factor (-1)^(cur-k). */
                int sign = ((cur - k) & 1) ? -1 : 1;

                /* Add contribution to sum. */
                seq_t term = binom * S_curr[k];
                if (sign < 0)
                    sum = sum - term;
                else
                    sum = sum + term;
            }

            /* Store the computed a(cur). */
            a_cache[cur] = sum;

            /* Prepare for next iteration: next row uses current as previous. */
            seq_t *tmp = S_prev;
            S_prev = S_curr;
            S_curr = tmp;
        }

        a_len = (size_t)n + 1;   /* we now have values up to index n */
    }

    return a_cache[(size_t)n];
}
