/*
 * A331259  Numerator of harmonic mean of 3 consecutive primes. Denominators
 *          are A331260.
 * https://oeis.org/A331259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) =
 *            numerator((3*prime(n)*prime(n+1)*prime(n+2))/(prime(n+1)*prime(n+2)+prime(n)*prime(n+2)+prime(n)*prime(n+1)))
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 187, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the numerator of the harmonic mean of three consecutive primes using incremental prime generation. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static idx_t *primes = NULL;   /* dynamically growing array of primes */
static size_t prime_cnt = 0;   /* number of primes stored so far */
static size_t prime_cap = 0;   /* current capacity of the array */

/* Ensure that at least `need` primes are stored in the array `primes`. */
static int ensure_primes(idx_t need)
{
    /* Grow the array if necessary. */
    if (prime_cap < (size_t)need) {
        size_t new_cap = prime_cap;
        if (new_cap == 0) new_cap = 128;
        while (new_cap < (size_t)need) new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        if (!tmp) return 0;        /* allocation failure */
        primes = tmp;
        prime_cap = new_cap;
    }

    /* Generate primes until we reach the required count. */
    while (prime_cnt < (size_t)need) {
        idx_t candidate;
        if (prime_cnt == 0) {
            candidate = 2;
        } else {
            candidate = primes[prime_cnt - 1] + 1;
            if ((candidate & 1) == 0) ++candidate;   /* skip even numbers */
        }

        /* Find the next prime by trial division. */
        for (;;) {
            bool is_prime = true;
            for (size_t i = 0; i < prime_cnt; ++i) {
                idx_t p = primes[i];
                if ((idx_t)p * (idx_t)p > candidate) break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                primes[prime_cnt++] = candidate;
                break;
            }
            candidate += 2;   /* test only odd candidates */
        }
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Need primes at indices n-1, n, n+1: require at least n+2 primes (0‑based). */
    size_t need = (size_t)n + 2;
    if (!ensure_primes((idx_t)need)) return 0;

    idx_t p1 = primes[(size_t)n - 1];
    idx_t p2 = primes[(size_t)n];
    idx_t p3 = primes[(size_t)n + 1];

    seq_t prime1 = (seq_t)p1;
    seq_t prime2 = (seq_t)p2;
    seq_t prime3 = (seq_t)p3;

    seq_t numerator;
    if (p1 % 3 == p2 % 3 && p2 % 3 == p3 % 3)
        numerator = prime1 * prime2 * prime3;
    else
        numerator = (seq_t)3 * prime1 * prime2 * prime3;

    seq_t denominator = prime2 * prime3 + prime1 * prime3 + prime1 * prime2;

    seq_t g = gcd(numerator, denominator);
    return numerator / g;
}
