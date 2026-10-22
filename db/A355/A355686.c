/*
 * A355686  Dirichlet inverse of A276150, where A276150(n) is the sum of
 *          digits when n is written in primorial base.
 * https://oeis.org/A355686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(1) = 1; for n > 1, * a(n) = - Σ_{d|n, d<n} A276150(n/d) * a(d)
 *            (Dirichlet inverse) * where A276150(n) is the sum of the digits
 *            of n written in primorial base. * * The function caches results
 *            in static arrays so that calls to a(n) * in any order remain
 *            O(n) per distinct new index
 * Range:     i8 n <= 494, i16 n <= 9745, i32 n <= 9745, i64 n <= 9745,
 *            i128 n <= 9745, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* utility for multiplication of seq_t values (handles big‑integer builds) */
#include "../utils/seq_mul.h"

/* seq_t and idx_t are supplied by the build system */
static const idx_t PRIMES[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL,
    73ULL, 79ULL, 83ULL, 89ULL, 97ULL
};
static const size_t NUM_PRIMES = sizeof(PRIMES) / sizeof(PRIMES[0]);

/* primorial numbers: prim[0] = 1; prim[k] = Π_{j<k} prime_j  */
static idx_t *prim = NULL;
static size_t prim_len = 0;
static bool prim_inited = false;

/* initialise primorial array, taking care not to overflow */
static void init_primorial(void)
{
    if (prim_inited)
        return;
    size_t max_sz = NUM_PRIMES + 1;
    prim = (idx_t *)malloc(max_sz * sizeof(idx_t));
    if (!prim) {
        /* memory failure unlikely in contest; terminate early */
        exit(EXIT_FAILURE);
    }
    prim[0] = 1ULL;
    idx_t prod = 1ULL;
    prim_len = 1;          /* prim[0] already set */
    for (size_t i = 0; i < NUM_PRIMES; ++i) {
        if (prod > (UINT64_MAX / PRIMES[i]))
            break;         /* would overflow */
        prod *= PRIMES[i];
        prim[prim_len++] = prod;
    }
    prim_inited = true;
}

/* sum of digits of n in primorial base */
static seq_t primorial_digit_sum(idx_t n)
{
    init_primorial();
    seq_t sum = 0;
    idx_t rem = n;
    for (int k = (int)prim_len - 1; k >= 1; --k) {
        if (prim[k] <= rem) {
            idx_t digit = rem / prim[k];
            sum += (seq_t)digit;
            rem -= digit * prim[k];
        }
    }
    sum += (seq_t)rem;  /* remaining 0 or 1 */
    return sum;
}

/* caches for f[n] = A276150(n) and a[n] (Dirichlet inverse) */
static seq_t *f_cache = NULL;
static seq_t *a_cache = NULL;
static idx_t cache_max = 0;

/* Compute a(n) for any n >= 1, memoising results for later calls */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* not a valid index – return 0 */

    if (n > cache_max) {
        /* grow caches to hold values up to n */
        size_t new_cap = (size_t)n + 1;
        seq_t *new_f = (seq_t *)malloc(new_cap * sizeof(seq_t));
        seq_t *new_a = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_f || !new_a) {
            /* memory failure unlikely; terminate */
            exit(EXIT_FAILURE);
        }
        /* copy old data if any */
        if (f_cache) {
            for (idx_t i = 1; i <= cache_max; ++i)
                new_f[i] = f_cache[i];
            free(f_cache);
        }
        if (a_cache) {
            for (idx_t i = 1; i <= cache_max; ++i)
                new_a[i] = a_cache[i];
            free(a_cache);
        }
        f_cache = new_f;
        a_cache = new_a;

        /* compute new entries */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i == 1) {
                f_cache[1] = (seq_t)1;
                a_cache[1] = (seq_t)1;
                continue;
            }
            f_cache[i] = primorial_digit_sum(i);

            seq_t sum = 0;
            /* sum over proper divisors d of i */
            for (idx_t d = 1; d < i; ++d) {
                if (i % d == 0) {
                    seq_t term = seq_mul(f_cache[i / d], a_cache[d]);
                    sum += term;
                }
            }
            a_cache[i] = -sum;
        }
        cache_max = n;
    }
    return a_cache[n];
}
