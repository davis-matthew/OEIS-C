/*
 * A390173  Smallest integer k with exactly n distinct prime factors such that
 *          the sum of those primes equals the sum of the exponents in its
 *          prime factorization.
 * https://oeis.org/A390173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * prime(n) * 2^(prime(n)-1), with a(1)=4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   get_prime, pow2, seq_alloc_new_array, seq_free_array, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the provided utilities get_prime, pow2, seq_mul, seq_alloc_new_array, and seq_free_array. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache   = NULL;   /* array of computed terms */
static idx_t  cap     = 0;      /* allocated capacity */
static idx_t  length  = 0;      /* number of terms already computed */

/* Return the n-th term of the sequence, n>=1.
   For n==0 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Ensure the cache can hold up to n terms. */
    if (n > cap) {
        idx_t new_cap = n * 2;                       /* oversize to reduce reallocs */
        seq_t *new_cache = seq_alloc_new_array((size_t)new_cap);

        if (cache) {
            /* Copy existing data to the new array. */
            for (idx_t i = 0; i < length; ++i)
                new_cache[i] = cache[i];
            seq_free_array(cache);                  /* free old array */
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = length + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)4;                    /* a(1) = 4 */
        } else {
            uint32_t p = get_prime(i);              /* i-th prime */
            seq_t   prime   = (seq_t)p;
            seq_t   two_pow = pow2((idx_t)(p - 1));   /* 2^(prime-1) */

            seq_t temp = seq_mul(cache[i - 2], prime); /* a(i-1) * prime */
            cache[i - 1] = seq_mul(temp, two_pow);      /* × 2^(prime-1) */
        }
    }

    length = n;
    return cache[n - 1];
}
