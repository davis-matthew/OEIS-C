/*
 * A365330  Expansion of e.g.f. x^3/(1-x-x^2-x^3)^2.
 * https://oeis.org/A365330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * T(n), where C[0]=1, C[1]=1, C[2]=2; C[n] = C[n-1] +
 *            C[n-2] + C[n-3] (compositions of n) T[0]=T[1]=T[2]=0; T[n] =
 *            T[n-1] + T[n-2] + T[n-3] + C[n-3] a(n) = factorial(n) * T[n]
 *            (gives the OEIS sequence A365330)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *fact  = NULL;   /* factorials */
static seq_t *c_arr = NULL;   /* C[n] – number of compositions of n with parts ≤3 */
static seq_t *T_arr = NULL;   /* T[n] – sum of number of 3‑blocks over all such compositions */
static size_t cache_sz = 0;   /* highest index computed (exclusive) */

/* Grow the caches up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_sz) return;        /* already computed */

    size_t new_sz = (size_t)n + 1;
    fact   = (seq_t *)realloc(fact,   new_sz * sizeof(seq_t));
    c_arr  = (seq_t *)realloc(c_arr,  new_sz * sizeof(seq_t));
    T_arr  = (seq_t *)realloc(T_arr,  new_sz * sizeof(seq_t));

    if (cache_sz == 0) {                         /* initialise first elements */
        fact[0]  = (seq_t)1;
        c_arr[0] = (seq_t)1;
        T_arr[0] = (seq_t)0;
        cache_sz = 1;
    }

    for (size_t i = cache_sz; i < new_sz; ++i) {
        fact[i] = fact[i - 1] * (seq_t)(i);        /* factorial */

        if (i == 1) {                            /* base cases for C and T */
            c_arr[i] = (seq_t)1;
            T_arr[i] = (seq_t)0;
        } else if (i == 2) {
            c_arr[i] = (seq_t)2;
            T_arr[i] = (seq_t)0;
        } else {                                 /* recurrence for i ≥ 3 */
            c_arr[i] = c_arr[i - 1] + c_arr[i - 2] + c_arr[i - 3];
            T_arr[i] = T_arr[i - 1] + T_arr[i - 2] + T_arr[i - 3] + c_arr[i - 3];
        }
    }
    cache_sz = new_sz;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return fact[n] * T_arr[n];
}
