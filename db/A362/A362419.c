/*
 * A362419  Partial sum of the first n even semiprimes.
 * https://oeis.org/A362419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (prime_1 + prime_2 + … + prime_n)
 * Range:     u8 n <= 9, u16 n <= 114, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the sum of the first n even semiprimes, i.e. 2*prime_i */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* Outer function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* guard for non‑positive indices */

    /* Static storage for primes and their prefix sums            */
    static idx_t    *primes   = NULL;
    static seq_t    *prefix   = NULL;
    static size_t    primes_cnt = 0;
    static idx_t    cur_limit  = 0;     /* upper bound used for growing   */

    /* ---------------------------------------------------------------- */
    /* If we have enough primes already, simply return the answer.     */
    if (primes_cnt >= (size_t)n) {
        if (!prefix) return 0;           /* safety check */
        seq_t sum = prefix[n];
        return (seq_t)2 * sum;
    }

    /* Helper to build primes up to at least `need` primes.          */
    const size_t need = (size_t)n;
    /* Estimate an initial limit for the sieve.                        */
    size_t limit;
    if (need < 2) limit = 20;            /* enough for the first prime */
    else {
        double dn = (double)need;
        limit = (size_t)(dn * (log(dn) + log(log(dn)))) + 10;
        if (limit < 20) limit = 20;
    }
    /* Grow the limit until we have enough primes.                     */
    while (1) {
        /* Boolean array for primes – use unsigned char for portability. */
        unsigned char *is_prime = (unsigned char*)calloc(limit + 1, 1);
        if (!is_prime) return 0;        /* allocation failure */

        is_prime[0] = 0;
        is_prime[1] = 0;
        for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;

        idx_t r = (idx_t)(sqrt((double)limit) + 0.5);
        for (idx_t p = 2; p <= r; ++p) {
            if (is_prime[p]) {
                for (idx_t m = p * p; m <= limit; m += p)
                    is_prime[m] = 0;
            }
        }

        /* Count primes found.                                          */
        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (is_prime[i]) ++cnt;

        /* If sufficient, collect them.                                */
        if (cnt >= need) {
            idx_t *new_primes = (idx_t*)malloc(cnt * sizeof(idx_t));
            if (!new_primes) { free(is_prime); return 0; }
            size_t idx = 0;
            for (idx_t i = 2; i <= limit; ++i)
                if (is_prime[i]) new_primes[idx++] = i;

            free(is_prime);

            /* Replace old tables if they exist.                         */
            free(primes);
            free(prefix);
            primes = new_primes;
            primes_cnt = cnt;
            cur_limit = limit;

            /* Build prefix sums of primes.                              */
            prefix = (seq_t*)calloc(primes_cnt + 1, sizeof(seq_t));
            if (!prefix) return 0;  /* allocation failure */
            prefix[0] = 0;
            for (size_t i = 1; i <= primes_cnt; ++i) {
                prefix[i] = prefix[i-1] + (seq_t)primes[i-1];
            }
            break;
        }

        /* Not enough primes – increase the limit and try again.      */
        free(is_prime);
        limit *= 2;
    }

    /* After construction, compute the required sum.                  */
    seq_t sum = prefix[n];
    return (seq_t)2 * sum;
}
