/*
 * A303691  a(n) is the number of 3-smooth numbers k such that prime(n)-k is
 *          also a prime number, where prime(n) stands for the n-th prime.
 * https://oeis.org/A303691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A303691
 * a(n) = number of 3‑smooth k <= prime(n) such that prime(n)-k is prime
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_small.h"

/* static cache of primes */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t   prime_count = 0;      /* number of primes stored */
static idx_t    prime_limit = 0;      /* current sieve limit */

/* obtain the n-th prime (1‑based) */
static seq_t get_nth_prime(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but never called with 0 */

    /* simple upper bound: for n >= 6, p_n < 20*n; for small n use a fixed small limit */
    idx_t limit = (n < 6) ? 15 : (n * 20);

    /* rebuild sieve if needed */
    if (prime_limit < limit || prime_count < n) {
        if (prime_list) free(prime_list);               /* discard old table */
        build_primes_upto(limit, &prime_list, &prime_count);
        prime_limit = limit;
    }

    return (seq_t)prime_list[n - 1]; /* 1‑based index */
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;                 /* a(1)=0 by definition */

    seq_t p = get_nth_prime(n);           /* prime(n) */
    seq_t count = 0;

    /* iterate over all 3‑smooth numbers k <= p */
    for (seq_t v = 1; v <= p; ) {         /* v = 3^j */
        for (seq_t k = v; k <= p; ) {     /* k = v * 2^i = 3^j * 2^i */
            if (is_prime_small(p - k)) count++;

            /* avoid overflow: stop if next doubling would exceed p */
            if (k > p / 2) break;
            k *= 2;
        }
        /* stop if next tripling would exceed p */
        if (v > p / 3) break;
        v *= 3;
    }

    return count;
}
