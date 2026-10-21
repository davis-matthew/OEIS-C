/*
 * A250437  Number of (n+1)X(2+1) 0..2 arrays with nondecreasing sum of every
 *          two consecutive values in every row and column.
 * https://oeis.org/A250437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(E) * f(O) * C(E+2,2) * C(O+2,2) where E = n/2 + 1
 *            (number of rows with even index) O = (n+1)/2 (number of rows
 *            with odd index) f(L) = number of pairs (u,v) of non‑decreasing
 *            0..2 sequences of length L with u_i <= v_i for every position i.
 *            f(L) is computed by a small DP over the 6 possibl...
 * Range:     u8 none, u16 n <= 4, u32 n <= 16, u64 n <= 132, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Transition data for the 6 admissible pairs (u,v). */
static const unsigned char pair_u[6] = {0,0,0,1,1,2};
static const unsigned char pair_v[6] = {0,1,2,1,2,2};

/* Cache for f(L) values.  Index 0 is unused. */
static seq_t *f_cache = NULL;
static idx_t  f_maxLen = 0;        /* largest L for which f_cache[L] is valid */

/* DP arrays that keep the number of chains ending in each pair.  These
   are static so they persist between calls to ensure_f() */
static seq_t dp_prev[6];
static seq_t dp_curr[6];
static bool  dp_initialized = false;

/* Ensure that f_cache contains values up to length 'need'.  The cache
   is expanded lazily, and the DP is extended from the current maximum. */
static void ensure_f(idx_t need)
{
    if (need <= f_maxLen)
        return;

    /* Allocate or grow the cache array. */
    size_t new_cap = (size_t)need + 1;            /* index 0..need */
    seq_t *tmp = f_cache;
    if (!tmp)
        tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        tmp = (seq_t *)realloc(tmp, new_cap * sizeof(seq_t));
    if (!tmp)
        return;                     /* allocation failed – leave unchanged */

    f_cache = tmp;

    /* Initialise DP if this is the first run. */
    if (!dp_initialized) {
        for (int i = 0; i < 6; ++i)
            dp_prev[i] = (seq_t)1;            /* length 1 chains of every pair */
        f_cache[1] = (seq_t)6;                 /* 6 admissible pairs */
        f_maxLen  = 1;
        dp_initialized = true;
    }

    /* Extend the DP from f_maxLen+1 up to 'need'. */
    for (idx_t len = f_maxLen + 1; len <= need; ++len) {
        /* reset dp_curr to zero */
        for (int i = 0; i < 6; ++i)
            dp_curr[i] = (seq_t)0;

        /* transition from every previous state j to current state i. */
        for (int j = 0; j < 6; ++j) {
            if (dp_prev[j] == 0)
                continue;
            unsigned char uj = pair_u[j];
            unsigned char vj = pair_v[j];

            for (int i = 0; i < 6; ++i) {
                if (uj <= pair_u[i] && vj <= pair_v[i])
                    dp_curr[i] += dp_prev[j];
            }
        }

        /* sum over all ending states to obtain f(len). */
        seq_t total = (seq_t)0;
        for (int i = 0; i < 6; ++i)
            total += dp_curr[i];
        f_cache[len] = total;

        /* prepare for next iteration. */
        for (int i = 0; i < 6; ++i)
            dp_prev[i] = dp_curr[i];
    }

    f_maxLen = need;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* value below the offset is undefined */

    /* number of even‑indexed and odd‑indexed rows. */
    idx_t E = n / 2 + 1;            /* rows 0,2,4,... */
    idx_t O = (n + 1) / 2;          /* rows 1,3,5,... */

    idx_t maxLen = (E > O) ? E : O;
    ensure_f(maxLen);

    seq_t fE = f_cache[E];
    seq_t fO = f_cache[O];

    /* binomial coefficient C(L+2,2) = (L+2)(L+1)/2 */
    seq_t bin_even = ( (seq_t)(E + 2) * (seq_t)(E + 1) ) / (seq_t)2;
    seq_t bin_odd  = ( (seq_t)(O + 2) * (seq_t)(O + 1) ) / (seq_t)2;

    seq_t result = fE;
    result *= bin_even;
    result *= fO;
    result *= bin_odd;

    return result;
}
