/*
 * A274985  a(n) = ([n]_phi! - [n]_{1-phi}!)/sqrt(5), where [n]_q! is the
 *          q-factorial, phi = (1+sqrt(5))/2.
 * https://oeis.org/A274985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A274985:
   a(n) = ([n]ₚₕᵢ! - [n]_{1-φ}!) / sqrt(5)   with φ = (1+sqrt(5))/2
   The function below computes the terms iteratively using the recurrence:
   [n]ₚₕᵢ! = [n-1]ₚₕᵢ! · [n]_φ
   where [n]_φ = (φ^(n+1) - φ).  Each term is represented as
   [k]_φ = (p + q √5)/2  with integer p,q, and the cumulative product
   is maintained as (A + a √5)/2.  The recurrence for (A,a) is:
        X = A·p + 5·a·q
        Y = A·q + a·p
        A' = X / 2
        a' = Y / 2
   which yields a(n) = a′ at step n.
   Fibonacci numbers (Fₙ) are used to obtain p and q:
        2·φ^(n+1) = (2F_n + F_{n+1}) + F_{n+1}√5
   so
        p = (2F_n + F_{n+1}) – 1
        q = F_{n+1} – 1
   The implementation stores previously computed terms in a static cache
   so that calls in arbitrary order remain efficient.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;

    /* static recurrence state */
    static seq_t A = 2;          /* 2·[current]_φ! = A + a√5   */
    static seq_t a_val = 0;      /* a component of the current term */
    static seq_t fib_prev = 0;   /* F_{k-1}  for the next computation */
    static seq_t fib_curr = 1;   /* F_k      for the next computation */

    /* initialise cache and constants if this is the first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed; return 0 */
        cache[0] = 0;                  /* a(0) = 0 */
        max_computed = 0;
        A = 2;
        a_val = 0;
        fib_prev = 0;
        fib_curr = 1;
    }

    /* grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* reallocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* Fibonacci step: compute F_{k+1} */
        seq_t F_next = fib_prev + fib_curr;

        /* determine p and q for [k]_φ */
        seq_t p = (seq_t)2 * fib_curr + F_next - 1;
        seq_t q = F_next - 1;

        /* recurrence update of A and a_val */
        seq_t X = A * p + (seq_t)5 * a_val * q;
        seq_t Y = A * q + a_val * p;
        A = X / 2;
        a_val = Y / 2;

        /* store the computed a(k) value */
        cache[k] = a_val;

        /* advance Fibonacci pointers for next iteration */
        fib_prev = fib_curr;
        fib_curr = F_next;
    }
    max_computed = n;
    return cache[n];
}
