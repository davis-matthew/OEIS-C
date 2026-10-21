/*
 * A244012  Numerators of rational approximations to sqrt(7) obtained from
 *          Newton's method.
 * https://oeis.org/A244012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   Numerators of Newton–Raphson approximations to sqrt(7).
 *            Recurrence: a(0)=2. For n ≥ 0 let p_n = a(n) and q_n the
 *            denominator of the nth rational approximation. Then p_{n+1} =
 *            p_n^2 + 7·q_n^2 q_{n+1} = 2·p_n·q_n with q_0 = 1. The function
 *            keeps a memoised table of (p_n, q_n) pairs so that calls in...
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *p_cache = NULL;      /* Numerators p_n */
static seq_t *q_cache = NULL;      /* Denominators q_n */
static idx_t cache_len = 0;        /* Number of terms currently stored */

/* Ensure the cache contains terms up to index n (inclusive). */
static void ensure_cached(idx_t n)
{
    if (cache_len == 0) {
        /* Initialise with n = 0 term. */
        p_cache = (seq_t *)malloc(sizeof(seq_t));
        q_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!p_cache || !q_cache) {
            /* Allocation failed – abort.  In a real project you would
               handle this more gracefully. */
            exit(1);
        }
        p_cache[0] = (seq_t)2;         /* a(0) */
        q_cache[0] = (seq_t)1;
        cache_len = 1;
    }

    while (cache_len <= n) {
        /* Compute next terms. */
        seq_t p_prev = p_cache[cache_len - 1];
        seq_t q_prev = q_cache[cache_len - 1];

        seq_t p_next = p_prev * p_prev + (seq_t)7 * q_prev * q_prev;
        seq_t q_next = (seq_t)2 * p_prev * q_prev;

        /* Grow storage to accommodate the new term. */
        seq_t *p_new = (seq_t *)realloc(p_cache,
                                        (cache_len + 1) * sizeof(seq_t));
        seq_t *q_new = (seq_t *)realloc(q_cache,
                                        (cache_len + 1) * sizeof(seq_t));
        if (!p_new || !q_new) {
            exit(1); /* Allocation failure. */
        }
        p_cache = p_new;
        q_cache = q_new;

        p_cache[cache_len] = p_next;
        q_cache[cache_len] = q_next;
        cache_len++;
    }
}

seq_t a(idx_t n)
{
    ensure_cached(n);
    return p_cache[n];
}
