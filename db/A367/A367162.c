/*
 * A367162  E.g.f. satisfies A(x) = 1 + A(x)^2 * (exp(x*A(x)) - 1).
 * https://oeis.org/A367162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (n+2*k)!/(n+k+1)! * Stirling2(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Ensure that the array pointed to by *ptr has at least 'need' elements
   and that *cap reflects its current capacity.  Reallocate if necessary. */
static void ensure_capacity(size_t need, size_t *cap, seq_t **ptr)
{
    if (*cap < need) {
        size_t new_cap = need * 2 + 2;
        seq_t *tmp = NULL;
        if (*ptr == NULL)
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
        *ptr = tmp;
        *cap = new_cap;
    }
}

/* Memoization of the sequence itself */
static seq_t *memo = NULL;    /* cache of a(n) values      */
static size_t memo_cap = 0;   /* capacity of memo          */
static idx_t  computed_max = 0; /* largest n already computed */

/* Tables for computing Stirling numbers of the second kind  */
static seq_t *S_prev = NULL;  /* previous Stirling row      */
static seq_t *S_curr = NULL;  /* current   Stirling row      */
static size_t S_prev_cap = 0; /* capacity of S_prev         */
static size_t S_curr_cap = 0; /* capacity of S_curr         */
static size_t S_prev_len = 0; /* current row length         */

/* Compute the OEIS sequence A367162 */
seq_t a(idx_t n)
{
    /* On the first call, initialise storage */
    if (memo == NULL) {
        ensure_capacity(1, &memo_cap, &memo);
        memo[0] = (seq_t)1;      /* a(0) = 1 */
        computed_max = 0;
    }

    if (S_prev == NULL) {
        ensure_capacity(1, &S_prev_cap, &S_prev);
        S_prev[0] = (seq_t)1;     /* S(0,0) = 1 */
        S_prev_len = 1;
    }

    /* If the requested value is already known, return it */
    if (n <= computed_max)
        return memo[n];

    /* Compute all rows up to n, accumulating the desired values */
    for (idx_t cur = computed_max + 1; cur <= n; ++cur) {
        /* Ensure current Stirling row has enough space */
        ensure_capacity((size_t)cur + 1, &S_curr_cap, &S_curr);

        /* Compute Stirling numbers for this row */
        for (idx_t k = 0; k <= cur; ++k) {
            if (k == 0)
                S_curr[0] = (seq_t)0;           /* S(cur,0) = 0 for cur > 0 */
            else if (k == cur)
                S_curr[k] = (seq_t)1;           /* S(cur,cur) = 1 */
            else
                S_curr[k] = S_prev[k - 1] + (seq_t)k * S_prev[k];
        }

        /* Compute a(cur) = Σ_{k=1..cur} S(cur,k) * (cur+2k)!/(cur+k+1)! */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t term = (seq_t)1;
            /* Product of integers from cur + k + 2 to cur + 2k */
            for (idx_t t = cur + k + 2; t <= cur + 2 * k; ++t)
                term *= (seq_t)t;
            sum += S_curr[k] * term;
        }

        /* Store computed value */
        ensure_capacity((size_t)cur + 1, &memo_cap, &memo);
        memo[cur] = sum;

        /* Rotate Stirling row buffers for next iteration */
        seq_t *tmp_ptr   = S_prev;
        S_prev          = S_curr;
        S_curr          = tmp_ptr;
        size_t tmp_cap   = S_prev_cap;
        S_prev_cap      = S_curr_cap;
        S_curr_cap      = tmp_cap;
        S_prev_len      = (size_t)cur + 1;
    }

    computed_max = n;
    return memo[n];
}
