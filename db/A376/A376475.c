/*
 * A376475  E.g.f. satisfies A(x) = exp( x^3*A(x)^3 / (1 - x*A(x)) ).
 * https://oeis.org/A376475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376475
 * a(0) = 1, a(1) = a(2) = 0.
 * For n ≥ 3:
 *     a(n) = n! * Σ_{k=1..⌊n/3⌋} (n+1)^{k-1} * C(n-2k-1, n-3k) / k!
 */
#include <stdlib.h>
#include <stdint.h>

static seq_t *fact_cache = NULL;   /* fact_cache[i] = i!  */
static idx_t fact_cache_size = 0;  /* current size of the cache */

/* Ensure that fact_cache contains factorials up to n */
static void ensure_factorial(idx_t n)
{
    if (fact_cache_size <= n) {
        idx_t new_size = n + 1;                /* need indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                     /* out of memory */
        fact_cache = tmp;
        if (fact_cache_size == 0) {
            fact_cache[0] = (seq_t)1;          /* 0! = 1   */
            fact_cache_size = 1;
        }
        for (idx_t i = fact_cache_size; i < new_size; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n < 3)   return (seq_t)0;

    ensure_factorial(n);

    idx_t kmax = n / 3;          /* floor(n/3) */
    seq_t term = fact_cache[n];  /* term for k = 1 */
    seq_t sum  = term;

    for (idx_t k = 1; k < kmax; ++k) {
        idx_t a_val = n - 2*k - 1;
        idx_t b_val = n - 3*k;

        if (sizeof(seq_t) <= 8) {
            unsigned __int128 num = (unsigned __int128)term * (unsigned __int128)(n + 1);
            num *= (unsigned __int128)b_val;
            num *= (unsigned __int128)(b_val - 1);
            num *= (unsigned __int128)(b_val - 2);

            unsigned __int128 den = (unsigned __int128)(k + 1) * (unsigned __int128)k;
            den *= (unsigned __int128)a_val;
            den *= (unsigned __int128)(a_val - 1);

            term = (seq_t)(num / den);
        } else {
            seq_t num = term * (seq_t)(n + 1);
            num *= (seq_t)b_val;
            num *= (seq_t)(b_val - 1);
            num *= (seq_t)(b_val - 2);

            seq_t den = (seq_t)(k + 1);
            den *= (seq_t)k;
            den *= (seq_t)a_val;
            den *= (seq_t)(a_val - 1);

            term = num / den;
        }
        sum += term;
    }

    return sum;
}
