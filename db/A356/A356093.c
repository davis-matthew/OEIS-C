/*
 * A356093  a(n) = numerator((prime(n)-1)/prime(n)#), where prime(n)# =
 *          A002110(n) is the n-th primorial.
 * https://oeis.org/A356093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator((prime(n)-1)/prime(n)#)
 * Range:     u8 n <= 54, u16 n <= 8618, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  prime(n)# is the primorial: product of first n primes */

/* This implementation keeps a small cache of primes up to the required
   size.  The primes themselves are stored as uint64_t, so that
   allocating the array with malloc/realloc is safe even when seq_t is
   an arbitrary-precision integer class.  The result is computed in
   seq_t using only integer arithmetic. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*---  Small utility: integer square root for uint64_t  --------------------*/
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        /* avoid overflow: compare mid to x/mid */
        if (mid == 0) {
            lo = mid + 1;
            continue;
        }
        if (mid > x / mid) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return hi;
}

/*---  Prime cache: array of uint64_t primes (1‑based indexing) -------------*/
static uint64_t *prime_cache = NULL;
static size_t prime_cnt   = 0;   /* number of stored primes */
static size_t prime_cap   = 0;   /* allocated capacity */

/* Ensure that prime_cache contains at least [size] primes. */
static void ensure_prime_cache(size_t size)
{
    while (prime_cnt < size) {
        uint64_t cand = (prime_cnt == 0) ? 2 : prime_cache[prime_cnt - 1] + 1;
        for (;;) {
            bool is_prime = true;
            uint64_t lim = isqrt_uint64(cand);
            for (size_t i = 0; i < prime_cnt && prime_cache[i] <= lim; ++i) {
                if (cand % prime_cache[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) break;
            ++cand;
        }

        /* Grow the array if necessary. */
        if (prime_cnt == prime_cap) {
            size_t new_cap = prime_cap == 0 ? 1 : prime_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
            if (!tmp) abort();           /* allocation failed */
            prime_cache = tmp;
            prime_cap   = new_cap;
        }

        /* Store the new prime. */
        prime_cache[prime_cnt++] = cand;
    }
}

/*---  Main sequence function  ---------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n==0; return 0 for safety */

    /* Ensure we have the n‑th prime. */
    ensure_prime_cache((size_t)n);

    uint64_t p = prime_cache[n - 1];          /* nth prime (1‑based) */
    uint64_t m = p - 1;                       /* value to factor */

    seq_t rad = 1;                            /* product of distinct primes dividing m */

    /* Factor m using the prime list up to sqrt(m). */
    uint64_t temp = m;
    for (size_t i = 0; i < prime_cnt && prime_cache[i] * prime_cache[i] <= temp; ++i) {
        uint64_t q = prime_cache[i];
        if (temp % q == 0) {
            rad *= (seq_t)q;                  /* multiply by distinct prime factor */
            while (temp % q == 0)             /* remove all powers of q */
                temp /= q;
        }
    }
    if (temp > 1) {
        rad *= (seq_t)temp;                   /* remaining prime factor > 1 */
    }

    /* The reduced numerator is (p‑1)/rad. */
    return ((seq_t)(p - 1)) / rad;
}
