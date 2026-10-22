/*
 * A357664  Expansion of e.g.f. sinh( (exp(2*x) - 1)/sqrt(2) )/sqrt(2).
 * https://oeis.org/A357664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} 2^(n-1-k) * Stirling2(n,2*k+1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

static seq_t *a_cache = NULL;       /* cache of a(n) values       */
static size_t cache_len = 0;        /* allocated length of cache   */
static idx_t  cur_max = 0;          /* maximum n that has been computed */

static seq_t *S_prev = NULL;        /* previous row of Stirling numbers */
static size_t S_prev_len = 0;       /* length of S_prev (previous_n + 1) */

/* Ensure a_cache is large enough to hold a(n) for all n <= needed */
static void ensure_a_cache(idx_t needed)
{
    if ((size_t)needed + 1 > cache_len) {
        size_t new_len = (size_t)needed + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        /* zero‑initialize the newly allocated part */
        for (size_t i = cache_len; i < new_len; ++i)
            tmp[i] = (seq_t)0;
        a_cache = tmp;
        cache_len = new_len;
    }
}

/* Initialise static data on first call (or after a_cache reset) */
static void init_static(void)
{
    if (!a_cache) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = (seq_t)0;   /* a(0) = 0 */
        cache_len = 1;
    }
    if (!S_prev) {
        S_prev = (seq_t *)malloc(sizeof(seq_t));
        if (!S_prev) abort();
        S_prev[0] = (seq_t)1;    /* S(0,0) = 1 */
        S_prev_len = 1;
    }
    cur_max = 0;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    init_static();
    ensure_a_cache(n);

    if (cur_max >= n) return a_cache[n];

    /* Build Stirling rows from cur_max+1 up to n */
    for (size_t next = (size_t)cur_max + 1; next <= (size_t)n; ++next) {
        size_t new_len = next + 1;
        seq_t *S_new = (seq_t *)calloc(new_len, sizeof(seq_t));
        if (!S_new) abort();

        /* S_new[0] = 0 is already zero via calloc */

        for (size_t m = 1; m <= next; ++m) {
            seq_t val = 0;
            if (m < S_prev_len) {
                val = S_prev[m] * (seq_t)m;
            }
            if (m - 1 < S_prev_len) {
                val += S_prev[m - 1];
            }
            S_new[m] = val;
        }

        /* Compute a(next) from the new Stirling row */
        seq_t total = (seq_t)0;
        size_t maxk = (next - 1) / 2;
        for (size_t k = 0; k <= maxk; ++k) {
            size_t m = 2 * k + 1;
            seq_t term = S_new[m];
            seq_t pow2shift = pow2((idx_t)(next - 1 - k));
            term = term * pow2shift;
            total = total + term;
        }
        a_cache[next] = total;

        /* Prepare for next iteration */
        free(S_prev);
        S_prev = S_new;
        S_prev_len = new_len;
    }

    cur_max = n;
    return a_cache[n];
}
