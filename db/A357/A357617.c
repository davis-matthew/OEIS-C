/*
 * A357617  Expansion of e.g.f. sinh( (exp(4*x) - 1)/4 ).
 * https://oeis.org/A357617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} 4^(n-1-2*k) *
 *            Stirling2(n,2*k+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* optional: for C++ big integer types, multiplication and division
   support is assumed to be defined for seq_t. */
static seq_t *a_cache = NULL;
static size_t a_cap   = 0;
static idx_t  current_max_n = 0;

static seq_t *S_prev = NULL;
static seq_t *S_curr = NULL;
static size_t S_cap  = 0;

/* --------------------------------------------------------------------- */
/* Compute 4^exp as a seq_t value.  exp >= 0.                              */
static seq_t pow4(idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= 4;
    return result;
}

/* --------------------------------------------------------------------- */
/* Main sequence entry point.                                            */
seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 0 by definition */
        return 0;

    /* --------------------------------------------------------------- */
    /* Grow the a_cache if necessary.                                   */
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = a_cap ? a_cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* out of memory */
        /* zero newly allocated slots */
        for (size_t i = a_cap; i < new_cap; ++i)
            tmp[i] = 0;
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* --------------------------------------------------------------- */
    /* Grow Stirling number buffers if necessary.                       */
    /* We need them sized at least current_max_n + 2 to store rows up   */
    /* until new n.                                                    */
    size_t needed_cap = (size_t)current_max_n + 2;
    if (needed_cap > S_cap) {
        size_t new_cap = S_cap ? S_cap << 1 : 1;
        while (new_cap < needed_cap) new_cap <<= 1;
        seq_t *tmp_prev = (seq_t *)realloc(S_prev, new_cap * sizeof(seq_t));
        seq_t *tmp_curr = (seq_t *)realloc(S_curr, new_cap * sizeof(seq_t));
        if (!tmp_prev || !tmp_curr) abort();
        /* zero newly allocated slots */
        for (size_t i = S_cap; i < new_cap; ++i) {
            tmp_prev[i] = 0;
            tmp_curr[i] = 0;
        }
        S_prev = tmp_prev;
        S_curr = tmp_curr;
        S_cap  = new_cap;
    }

    /* --------------------------------------------------------------- */
    /* Initialise the first Stirling row if this is the very first    */
    /* computation (current_max_n == 0).                               */
    if (current_max_n == 0) {
        S_prev[0] = 1;   /* S(0,0) = 1 */
    }

    /* --------------------------------------------------------------- */
    /* Incrementally compute rows from current_max_n+1 up to n and    */
    /* compute the corresponding a(m).                                */
    for (idx_t m = current_max_n + 1; m <= n; ++m) {
        S_curr[0] = 0;
        for (idx_t k = 1; k <= m; ++k)
            S_curr[k] = (seq_t)k * S_prev[k] + S_prev[k - 1];

        /* Compute a(m) using the formula.  Iterate odd j ascending. */
        seq_t sum = 0;
        seq_t pow = pow4(m - 1);   /* 4^(m-1)      */
        for (idx_t j = 1; j <= m; j += 2) {
            sum += pow * S_curr[j];
            pow /= 16;             /* next odd reduces exponent by 2 */
        }
        a_cache[m] = sum;

        /* Swap buffers for next iteration. */
        seq_t *tmp = S_prev;
        S_prev = S_curr;
        S_curr = tmp;
    }

    current_max_n = n;
    return a_cache[n];
}
