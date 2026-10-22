/*
 * A375796  Expansion of e.g.f. 1/(1 - (exp(x^3) - 1)/x^2).
 * https://oeis.org/A375796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (n-3k)! *
 *            Stirling2(n-2k,n-3k)/(n-2k)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code implements the equivalent recurrence derived from the e.g.f. */

/*  a(n) = n! * Σ_{k≥1} a(n-3k+2) / ((n-3k+2)!·k!)  */

#include <stdlib.h>
#include <stdint.h>

static seq_t *fact_arr   = NULL;  /* factorials 0!.. */
static seq_t *a_cache    = NULL;  /* cached a(n)  */
static size_t cache_cap  = 0;     /* number of elements stored in arrays */

/* Ensure factorials and a(n) are computed up to n */
static void compute_up_to(idx_t n) {
    if (n < cache_cap) return;

    size_t new_cap = (size_t)(n + 1);
    fact_arr = (seq_t *)realloc(fact_arr, new_cap * sizeof(seq_t));
    a_cache  = (seq_t *)realloc(a_cache,  new_cap * sizeof(seq_t));

    /* Compute missing factorials and a(n) */
    for (idx_t i = cache_cap; i <= n; ++i) {
        if (i == 0) {
            fact_arr[i] = 1;
            a_cache[i]  = 1;
            continue;
        }

        /* factorial(i) = factorial(i-1) * i */
        fact_arr[i] = fact_arr[i-1] * (seq_t)i;

        /* Compute a(i) using the recurrence */
        seq_t sum = 0;
        idx_t max_k = (i + 2) / 3;          /* floor((i+2)/3)  */
        for (idx_t k = 1; k <= max_k; ++k) {
            idx_t t = i - 3*k + 2;          /* n - 3k + 2  */
            seq_t term = fact_arr[i];
            term = term / fact_arr[t];
            term = term / fact_arr[k];
            term = term * a_cache[t];
            sum = sum + term;
        }
        a_cache[i] = sum;
    }

    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    compute_up_to(n);
    return a_cache[n];
}
