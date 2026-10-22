/*
 * A344746  a(n) = Sum_{k=1..n} d(k) * k^c(n/k), where c(n) = 1 - ceiling(n) +
 *          floor(n).
 * https://oeis.org/A344746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} d(k) * k^c(n/k), where c(x) = 1 - ceil(x) +
 *            floor(x); equivalently a(n) = Σ_{k=1..n} d(k) + Σ_{d|n} d(d) *
 *            (d-1)
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a small memoisation buffer that holds the values
   of divisor counts d(k), their cumulative sums D(k), and the result
   a(k) for all k ≤ current maximum.  Calls for decreasing n are
   handled correctly because the buffer is never shrunk.  The routine
   runs in O(max_n log max_n) time for each major resize, which is
   easily fast enough for n ≤ 10 000.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t max_n = 0;
static seq_t *a_cache = NULL;  /* a(k) for k = 0..max_n */
static idx_t *d_cache = NULL;  /* divisor counts d(k) for k = 0..max_n */
static seq_t *D_cache = NULL;  /* cumulative sums Σ_{i≤k} d(i) */

/* Recompute all arrays up to at least n (no shrinking).  */
static void ensure_cap(idx_t n)
{
    if (n <= max_n) return;

    /* Allocate fresh arrays of size n+1.  Use new[] in C++ and malloc in C
       (no constructors are required for integer types).  */
    seq_t *new_a = NULL;
    idx_t *new_d = NULL;
    seq_t *new_D = NULL;
#ifdef __cplusplus
    new_a = new seq_t[n + 1];
    new_d = new idx_t[n + 1];
    new_D = new seq_t[n + 1];
#else
    new_a = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    new_d = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    new_D = (seq_t *)malloc((n + 1) * sizeof(seq_t));
#endif
    if (!new_a || !new_d || !new_D) abort();

    /* initialise to zero */
    for (idx_t i = 0; i <= n; ++i) {
        new_a[i] = 0;
        new_d[i] = 0;
        new_D[i] = 0;
    }

    /* Compute divisor counts d(k) by the classic sieve. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            ++new_d[j];
        }
    }

    /* Compute cumulative sums D(k) = Σ_{i≤k} d(i).  */
    for (idx_t i = 1; i <= n; ++i) {
        new_D[i] = new_D[i - 1] + (seq_t)new_d[i];
    }

    /* Compute a(k).  a(k) = D(k) + Σ_{d|k} d(d)*(d-1).  */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = new_D[i];          /* Σ_{k=1..i} d(k) */
        /* Add the extra part from divisors.  */
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                /* divisor d */
                sum += (seq_t)new_d[d] * ((seq_t)d - 1);
                /* complementary divisor i/d, if distinct */
                idx_t other = i / d;
                if (other != d) {
                    sum += (seq_t)new_d[other] * ((seq_t)other - 1);
                }
            }
        }
        new_a[i] = sum;
    }

    /* Free old arrays */
#ifdef __cplusplus
    delete[] a_cache;
    delete[] d_cache;
    delete[] D_cache;
#else
    free(a_cache);
    free(d_cache);
    free(D_cache);
#endif

    /* Install new arrays */
    a_cache = new_a;
    d_cache = new_d;
    D_cache = new_D;
    max_n   = n;
}

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for n<1, but safe */
    ensure_cap(n);
    return a_cache[n];
}
