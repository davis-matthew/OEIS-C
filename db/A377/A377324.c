/*
 * A377324  E.g.f. satisfies A(x) = 1 + (exp(x*A(x)^3) - 1)/A(x).
 * https://oeis.org/A377324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS entry A377324, offset 0
    Formula: a(n) = Sum_{k=0..n} (3*n-k)!/(3*n-2*k+1)! * Stirling2(n,k)
*/

#include <stdlib.h>
#include <stddef.h>

static seq_t *a_cache   = NULL;  /* cache for a(n) values          */
static size_t a_cap     = 0;      /* capacity of a_cache             */
static idx_t  a_max     = (idx_t)-1;   /* highest index cached */

static seq_t *S = NULL;          /* current row of Stirling numbers  */
static size_t S_cap   = 0;        /* capacity of S                   */
static idx_t  S_n     = 0;        /* current n for which S holds row */

/* Ensure a_cache has space for indices 0..n               */
static void ensure_a_cache(idx_t n)
{
    size_t needed = n + 1;
    if (a_cap < needed) {
        seq_t *tmp = (seq_t *)realloc(a_cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_cap   = needed;
    }
}

/* Ensure S has space for indices 0..n               */
static void ensure_S_capacity(idx_t n)
{
    size_t needed = n + 1;
    if (S_cap < needed) {
        seq_t *tmp = (seq_t *)realloc(S, needed * sizeof(seq_t));
        if (!tmp) abort();
        S      = tmp;
        S_cap  = needed;
    }
}

/* Compute a(n) for any n, memoizing results            */
seq_t a(idx_t n)
{
    /* ---------- Initialization ------------------------------------ */
    if (!a_cache) {                       /* first call: allocate a_cache   */
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = (seq_t)1;             /* a(0) = 1                       */
        a_cap     = 1;
        a_max     = 0;
    }
    if (!S) {                            /* first call: allocate S (row 0)  */
        S      = (seq_t *)malloc(sizeof(seq_t));
        if (!S) abort();
        S[0]     = (seq_t)1;             /* S(0,0) = 1                     */
        S_cap    = 1;
        S_n      = 0;
    }

    /* ---------- Return if already cached -------------------------- */
    if (n <= a_max) return a_cache[n];

    /* ---------- Compute rows up to n ----------------------------- */
    for (idx_t cur_n = a_max + 1; cur_n <= n; ++cur_n) {

        /* ensure storage for new row */
        ensure_S_capacity(cur_n);
        if (cur_n > S_n) {
            /* Expand S array; new entries are zero-initialised
               (they will be overwritten by the recurrence).          */
            for (idx_t i = S_n + 1; i <= cur_n; ++i) S[i] = (seq_t)0;
            S_n = cur_n;
        }

        /* Update Stirling row in place (reverse order) */
        for (idx_t k = cur_n; k >= 1; --k) {
            seq_t k_seq = (seq_t)k;
            S[k] = k_seq * S[k] + S[k - 1];
        }
        S[0] = (seq_t)0;   /* S(n,0)=0 for n>0 */

        /* Compute a(cur_n) using the product recurrence for t */
        seq_t sum = (seq_t)0;
        seq_t t   = (seq_t)1;      /* t_1 = 1 */

        for (idx_t k = 1; k <= cur_n; ++k) {
            sum += S[k] * t;
            if (k < cur_n) {
                /* Update t for the next k+1 (k starts at 1) */
                idx_t kp1     = k + 1;
                seq_t num1 = (seq_t)(3 * cur_n - 2 * kp1 + 2);
                seq_t num2 = (seq_t)(3 * cur_n - 2 * kp1 + 3);
                seq_t den  = (seq_t)(3 * cur_n - kp1 + 1);
                t = (t * num1 * num2) / den;
            }
        }

        /* Cache the result */
        ensure_a_cache(cur_n);
        a_cache[cur_n] = sum;
        a_max = cur_n;
    }

    return a_cache[n];
}
