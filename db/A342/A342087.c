/*
 * A342087  Number of chains of divisors starting with n and having no
 *          adjacent parts x <= y^2.
 * https://oeis.org/A342087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: h(1)=1; for n>1, h(n)=∑_{d|n, d^2 < n} h(d).
   Then a(n)=1 if n==1 else 2*h(n). */
#include <stdlib.h>
#include <math.h>

static seq_t *h_cache = NULL;
static size_t h_cap = 0;
static idx_t h_computed_up_to = 0;

/* Ensure that h_cache[1..n] are computed. */
static void ensure_h_up_to(idx_t n)
{
    /* Allocate or enlarge the cache as needed. */
    if (n + 1 > h_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) abort();          /* out of memory */
        if (h_cache) {
            for (size_t i = 0; i < h_cap; ++i)
                new_arr[i] = h_cache[i];
            free(h_cache);
        }
        h_cache = new_arr;
        h_cap   = new_cap;
    }

    /* First‑time initialization. */
    if (h_computed_up_to == 0) {
        h_cache[1] = 1;
        h_computed_up_to = 1;
    }

    /* Compute h for indices greater than current maximum. */
    for (idx_t i = h_computed_up_to + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* floor(sqrt(i-1)) – cast via long double for full precision. */
        idx_t lim = (idx_t)sqrtl((long double)(i - 1));
        for (idx_t d = 1; d <= lim; ++d) {
            if (i % d == 0) {
                sum += h_cache[d];
            }
        }
        h_cache[i] = sum;
    }
    h_computed_up_to = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    if (n == 1)
        return 1;            /* special case */
    ensure_h_up_to(n);
    return (seq_t)2 * h_cache[n];
}
