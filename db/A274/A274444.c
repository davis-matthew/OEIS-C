/*
 * A274444  a(n) = smallest composite squarefree number k such that (p-n) |
 *          (k+1) for all primes dividing k.
 * https://oeis.org/A274444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest composite squarefree number k such that (p-n) |
 *            (k+1) for all prime divisors p of k
 * Range:     u8 n <= 19, u16 n <= 185, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Global prime table */
static uint64_t *primes = NULL;      /* array of primes */
static size_t   primes_count = 0;
static size_t   prime_limit   = 0;

/* Rebuild the prime table up to 'limit' (inclusive) */
static void ensure_primes_up_to(size_t limit)
{
    if (prime_limit >= limit) return;

    size_t size = limit + 1;
    char *is_prime = (char *)malloc(size);
    if (!is_prime) return;                     /* allocation failed */
    memset(is_prime, 1, size);
    if (size > 0) is_prime[0] = 0;
    if (size > 1) is_prime[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* free old table */
    if (primes) {
        free(primes);
        primes = NULL;
    }

    primes_count = 0;
    primes = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (!primes) {
        prime_limit = limit;
        free(is_prime);
        return;
    }

    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[primes_count++] = (uint64_t)i;

    prime_limit = limit;
    free(is_prime);
}

/* Euclidean gcd for uint64_t */
static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence entry point */
seq_t a(idx_t n)
{
    /* Start with a reasonable prime limit; grow if necessary */
    size_t limit = (n > 100000ULL ? (size_t)n + 1000 : 100000);
    ensure_primes_up_to(limit);

    /* maximum representable value for seq_t (used for overflow checks) */
    seq_t   max_seq     = ~((seq_t)0);
    unsigned __int128 max_seq128 = (unsigned __int128)max_seq;

    while (1) {
        seq_t bestK = 0;

        for (size_t i = 0; i < primes_count; ++i) {
            uint64_t p = primes[i];
            if (p == (uint64_t)n) continue;        /* prime equals n => divisor zero */

            /* If a smaller product is impossible with this p, skip remaining ps */
            if (bestK != 0 && i + 1 < primes_count) {
                unsigned __int128 minProd = (unsigned __int128)p * (unsigned __int128)primes[i + 1];
                if (minProd >= (unsigned __int128)bestK) break;
            }

            for (size_t j = i + 1; j < primes_count; ++j) {
                uint64_t q = primes[j];
                if (q == (uint64_t)n) continue;    /* prime equals n */

                unsigned __int128 prod128 = (unsigned __int128)p * (unsigned __int128)q;
                if (prod128 > max_seq128) continue;   /* would overflow seq_t, ignore */

                if (bestK != 0 && prod128 >= (unsigned __int128)bestK) break;

                /* absolute differences */
                int64_t d1 = (int64_t)p - (int64_t)n;
                if (d1 < 0) d1 = -d1;
                if (d1 == 0) continue;

                int64_t d2 = (int64_t)q - (int64_t)n;
                if (d2 < 0) d2 = -d2;
                if (d2 == 0) continue;

                uint64_t d1u = (uint64_t)d1;
                uint64_t d2u = (uint64_t)d2;
                uint64_t g   = gcd64(d1u, d2u);
                uint64_t lcm = d1u / g * d2u;

                seq_t  k       = (seq_t)prod128;
                seq_t  lcm_seq = (seq_t)lcm;

                if (((k + 1) % lcm_seq) == 0) {
                    if (bestK == 0 || k < bestK) bestK = k;
                }
            }
        }

        if (bestK != 0) return bestK;          /* answer found */

        /* No answer yet – double the prime range and retry */
        limit *= 2;
        ensure_primes_up_to(limit);
    }
}
