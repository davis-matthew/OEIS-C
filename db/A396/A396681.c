/*
 * A396681  Number of iterations for the n-th prime to reach 2 under the
 *          function f(p) = smallest odd prime factor of 5*p+1 or 2 if 5*p+1
 *          is a power of 2.
 * https://oeis.org/A396681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A396681: number of iterations for the n-th prime to reach
 * 2 under the map 
 *
 *    f(p) = 2                                 if 5p+1 is a power of 2
 *    f(p) = smallest prime factor of odd part   otherwise
 *
 * (with f(2)=2).  a(n) counts the number of applications of f starting from
 * the n‑th prime until the value 2 is reached.
 */

#include <stdlib.h>
#include <stdint.h>

/* Convert a static array of uint64_t primes that grows as needed. */
static uint64_t *primes = NULL;   /* array of primes */
static size_t   primes_cnt = 0;   /* number of primes stored */

/* Simple sieve that generates all primes up to <limit> inclusively. 
 * The sieve is recomputed only when a larger limit is required. */
static void ensure_primes(idx_t n) {
    /* Quick exit if already have enough primes. */
    if (primes_cnt >= (size_t)n)
        return;

    /* Choose a sieve limit that surely contains the n-th prime.  20*n
     * is a safe overestimate for all reasonable n (e.g. n=10^6 gives
     * limit≈20 000 000). */
    size_t limit = (n < 6) ? 15 : (size_t)n * 20 + 100;

    /* Rebuild the sieve if the new limit is larger. */
    if (!primes || limit > primes_cnt) {
        free(primes);
        primes = NULL;
        primes_cnt = 0;

        /* Allocate the sieve array. */
        unsigned char *is_composite = (unsigned char*)calloc(limit + 1, 1);
        if (!is_composite)
            return;   /* allocation failure – abort silently */

        size_t capacity = 100;            /* initial capacity for prime array */
        primes = (uint64_t*)malloc(capacity * sizeof(uint64_t));
        if (!primes) {
            free(is_composite);
            return;
        }

        for (size_t i = 2; i <= limit; ++i) {
            if (!is_composite[i]) {
                /* Store prime */
                if (primes_cnt >= capacity) {
                    capacity *= 2;
                    uint64_t *tmp = (uint64_t*)realloc(primes, capacity * sizeof(uint64_t));
                    if (!tmp) {
                        free(is_composite);
                        free(primes);
                        primes = NULL;
                        primes_cnt = 0;
                        return;
                    }
                    primes = tmp;
                }
                primes[primes_cnt++] = (uint64_t)i;

                /* Mark multiples */
                if (i <= limit / i)
                    for (size_t j = i * i; j <= limit; j += i)
                        is_composite[j] = 1;
            }
        }
        free(is_composite);
    }
}

/* Smallest prime factor of an odd number m > 1 using the primes array.
 * If m itself is prime, m is returned.  m==1 is not handled by this
 * function; the caller must treat that case separately. */
static seq_t smallest_prime_factor(seq_t m) {
    if (m <= 1)
        return 2;   /* shouldn't happen */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t pp = (seq_t)p * p;
        if (pp > m)
            break;
        if (m % (seq_t)p == 0)
            return (seq_t)p;
    }
    /* No factor found in the precomputed range – m itself is prime. */
    return m;
}

seq_t a(idx_t n) {
    if (n < 1)
        return 0;   /* undefined for n<1, return 0 */

    ensure_primes(n);

    seq_t steps = 0;
    seq_t p = (seq_t)primes[n - 1];   /* n-th prime (1‑based) */

    /* Iterate until we reach 2. */
    while (p != 2) {
        seq_t N = (seq_t)5 * p + 1;        /* compute 5p+1 */
        /* Remove all factors of 2 to obtain the odd part. */
        while ((N & 1) == 0)
            N >>= 1;
        seq_t m = N;
        seq_t next;
        if (m <= 1)
            next = 2;                       /* 5p+1 was a power of 2 */
        else
            next = smallest_prime_factor(m);
        p = next;
        ++steps;
    }
    return steps;
}
