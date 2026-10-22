/*
 * A323910  Dirichlet inverse of the deficiency of n, A033879.
 * https://oeis.org/A323910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = - ∑_{d|n, d<n} ( 2*(n/d) - σ(n/d) ) * a(d) where σ(m) =
 *            sum of divisors of m and a(1) = 1
 * Range:     i8 n <= 130, i16 n <= 9980, i32 n <= 9980, i64 n <= 9980,
 *            i128 n <= 9980, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static cache for sigma and the sequence; indices start at 1 */
static seq_t* sigma_arr = NULL;
static seq_t* a_arr     = NULL;
static idx_t  cur_cap   = 0;

/* Extend the cache so that values up to n are available. */
static void ensure_upto(idx_t n)
{
    if (n <= cur_cap) return;          /* already computed */

    /* Allocate new arrays for sigma and a up to n. */
    seq_t* new_sigma = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!new_sigma) { a_arr = NULL; return; }  /* allocation failure: abort */

    /* Compute σ(m) for all m ≤ n via a simple sieve. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            new_sigma[j] += (seq_t)i;
        }
    }

    seq_t* new_a = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!new_a) { free(new_sigma); a_arr = NULL; return; }

    new_a[1] = (seq_t)1;   /* base value */

    /* Compute a(i) for all i from 2 to n using the recurrence. */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t d = 1; d*d <= i; ++d) {
            if (i % d) continue;
            idx_t k = i / d;                /* complementary divisor */

            /* Term for divisor d */
            if (d < i) {
                seq_t f = (seq_t)2 * (seq_t)k - new_sigma[k];
                sum += f * new_a[d];
            }

            /* Term for divisor k, different from d */
            if (k < i && k != d) {
                seq_t f = (seq_t)2 * (seq_t)d - new_sigma[d];
                sum += f * new_a[k];
            }
        }
        new_a[i] = -sum;
    }

    /* Replace old arrays with the newly computed ones. */
    free(sigma_arr);
    free(a_arr);
    sigma_arr = new_sigma;
    a_arr     = new_a;
    cur_cap   = n;
}

/* Public function requested by the benchmarking framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, harmless value */
    ensure_upto(n);
    return a_arr[n];
}
