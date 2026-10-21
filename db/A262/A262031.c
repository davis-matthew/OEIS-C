/*
 * A262031  Numerator of partial sums of a reordered alternating harmonic
 *          series.
 * https://oeis.org/A262031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the nth partial sum of the reordered
 *            alternating harmonic series. The series terms are: c(k) =
 *            3/(4*k+3) if k % 3 == 0 = 3/(4*k+5) if k % 3 == 1 = -3/(2*(k+1))
 *            if k % 3 == 2 s(n) = sum_{k=0..n} c(k), a(n) = numerator( s(n) )
 *            in lowest terms. The implementation caches intermedi...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *num_cache = NULL;   /* numerator of s(k) for each k */
static seq_t *den_cache = NULL;   /* denominator of s(k) for each k */
static size_t cache_size = 0;     /* number of cached entries, >= 1 when initialized */

/* Helper: ensure the cache holds entries for indices 0..n inclusive. */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_size) return;           /* already enough */

    size_t new_size = (n + 1) * 2;                       /* exponential growth */
    if (new_size == 0) new_size = 1;                     /* for n == UINT64_MAX */

    seq_t *new_num = (seq_t*)realloc(num_cache, new_size * sizeof(seq_t));
    seq_t *new_den = (seq_t*)realloc(den_cache, new_size * sizeof(seq_t));
    if (!new_num || !new_den) {
        /* allocation failed; terminate as the sequence cannot be produced */
        exit(1);
    }
    num_cache = new_num;
    den_cache = new_den;

    /* Initialise first entry if this is the first allocation */
    if (cache_size == 0) {
        num_cache[0] = 1;        /* after k = 0, the sum is 1 */
        den_cache[0] = 1;
        cache_size = 1;
    }

    /* Compute new entries up to n */
    for (idx_t k = cache_size; k <= n; ++k) {
        idx_t prev = k - 1;
        seq_t prev_num = num_cache[prev];
        seq_t prev_den = den_cache[prev];

        idx_t mod = k % 3;
        seq_t q;
        seq_t new_num, new_den;

        if (mod == 0) {                    /* 3/(4*k+3) */
            q = (seq_t)4 * k + 3;
            new_num = prev_num * q + prev_den * 3;
            new_den = prev_den * q;
        } else if (mod == 1) {             /* 3/(4*k+5) */
            q = (seq_t)4 * k + 5;
            new_num = prev_num * q + prev_den * 3;
            new_den = prev_den * q;
        } else {                           /* -3/(2*(k+1)) */
            q = (seq_t)2 * (k + 1);
            new_num = prev_num * q - prev_den * 3;
            new_den = prev_den * q;
        }

        /* Reduce the fraction to lowest terms */
        seq_t g = gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;

        num_cache[k] = new_num;
        den_cache[k] = new_den;
    }

    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return num_cache[n];
}
