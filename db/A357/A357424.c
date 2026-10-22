/*
 * A357424  E.g.f. satisfies A(x) * exp(A(x)) = exp(x * exp(A(x))) - 1.
 * https://oeis.org/A357424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n-k)^(k-1) * Stirling2(n,k) (OEIS A357424)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/seq_pow.h"

static seq_t *a_cache   = NULL;   /* memoised a(n) values                */
static size_t a_cap     = 0;       /* capacity of a_cache array           */
static size_t a_len     = 0;       /* number of stored terms in a_cache   */

static seq_t *S_prev   = NULL;    /* S(n-1,k)  array for current n-1    */
static seq_t *S_curr   = NULL;    /* temporary array for S(n,k)         */
static size_t S_prev_cap = 0;     /* capacity of S_prev array           */
static size_t S_curr_cap = 0;     /* capacity of S_curr array           */
static idx_t  S_prev_len = 0;     /* current n for which S_prev stores  */

/* ------------------------------------------------------------------ */
/* Utility: enlarge an array if needed, copying existing elements.  */
/* ------------------------------------------------------------------ */
static void grow_seqs(seq_t **arr, size_t *cap, size_t len, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 1;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_arr = seq_alloc_new_array(new_cap);
    if (*arr && len > 0)
        memcpy(new_arr, *arr, len * sizeof(seq_t));
    if (*arr)
        seq_free_array(*arr);
    *arr = new_arr;
    *cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Compute a(m) for all m up to n (inclusive) and memoise them.      */
/* ------------------------------------------------------------------ */
static void compute_to(idx_t n)
{
    /* ---------------------------------------------------------------- */
    /* 1. Ensure a_cache has space up to n                               */
    /* ---------------------------------------------------------------- */
    grow_seqs(&a_cache, &a_cap, a_len, (size_t)n + 1);
    if (a_len == 0) {          /* initialise a_cache[0] */
        a_cache[0] = (seq_t)0;
        a_len = 1;
    }

    /* ---------------------------------------------------------------- */
    /* 2. Initialise Stirling‑number arrays on first call                */
    /* ---------------------------------------------------------------- */
    if (S_prev == NULL) {
        S_prev = seq_alloc_new_array(1);
        S_prev[0] = (seq_t)1;      /* S(0,0) = 1 */
        S_prev_cap = 1;
        S_prev_len = 0;
    }
    if (S_curr == NULL) {
        S_curr = seq_alloc_new_array(1);
        S_curr_cap = 1;
    }

    /* ---------------------------------------------------------------- */
    /* 3. Iterate m = S_prev_len+1 .. n to compute new rows of Stirling*/
    /* ---------------------------------------------------------------- */
    for (idx_t m = S_prev_len + 1; m <= n; ++m) {
        /* Ensure S_prev and S_curr can hold indices up to m       */
        grow_seqs(&S_prev, &S_prev_cap, (size_t)m + 1, (size_t)m + 1);
        grow_seqs(&S_curr, &S_curr_cap, (size_t)m + 1, (size_t)m + 1);

        S_curr[0] = (seq_t)0;                   /* S(m,0) = 0 for m>0 */
        for (idx_t k = m; k >= 1; --k) {
            seq_t term1 = (k > 0 && k - 1 < S_prev_len + 1) ? S_prev[k - 1] : (seq_t)0;
            seq_t term2 = (k <= S_prev_len) ? S_prev[k] * (seq_t)k : (seq_t)0;
            S_curr[k] = term1 + term2;
        }

        /* ---------------------------------------------------------------- */
        /* 4. Compute a(m) using the definition                               */
        /* ---------------------------------------------------------------- */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t base = (seq_t)(m - k);
            idx_t exp  = k - 1;
            seq_t pow_term;
            if (base == (seq_t)0 && exp == 0)   /* convention 0^0 = 1 */
                pow_term = (seq_t)1;
            else
                pow_term = seq_pow(base, exp);
            sum += S_curr[k] * pow_term;
        }
        a_cache[m] = sum;

        /* ---------------------------------------------------------------- */
        /* 5. Prepare for next iteration: S_prev <- S_curr                 */
        /* ---------------------------------------------------------------- */
        seq_t *tmp = S_prev;
        S_prev = S_curr;
        S_curr = tmp;
        S_prev_len = m;
    }
    a_len = (size_t)n + 1;
}

/* ------------------------------------------------------------------ */
/* Public function: returns a(n) for any  n >= 0                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (a_cache == NULL || (size_t)n >= a_len)
        compute_to(n);

    return a_cache[n];
}
