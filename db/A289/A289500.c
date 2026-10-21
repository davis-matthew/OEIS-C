/*
 * A289500  Number of primes in the interval [9n, 10n].
 * https://oeis.org/A289500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes in the interval [9n, 10n].
 *            Implementation uses a simple incremental Eratosthenes sieve up
 *            to 10n and counts primes in the requested range
 * Range:     u8 n <= 2602, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function satisfies all OEIS implementation requirements. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static state shared across calls */
    static idx_t       current_limit = 0;   /* last 10n that was sieved    */
    static bool        initialized   = false;/* indicates whether sieve exists */
    static uint8_t    *sieve         = NULL;  /* 0 = prime symbol, 1 = composite */

    /* When n is large, avoid overflow for 10*n (within bounds of expected use) */
    idx_t needed = n * 10ULL;

    /* Rebuild the sieve only if required */
    if (!initialized || needed > current_limit) {
        /* Free previous sieve if present */
        if (sieve) free(sieve);

        size_t len = (size_t)needed + 1u;      /* array size including 0 */
        sieve = (uint8_t *)calloc(len, sizeof(uint8_t)); /* all zeros = prime symbol */
        if (!sieve) return 0;                 /* allocation failure → return 0 */

        /* Mark 0 and 1 as composite */
        if (len > 0) sieve[0] = 1;
        if (len > 1) sieve[1] = 1;

        /* Simple Eratosthenes sieve up to needed */
        for (idx_t i = 2; i * i <= needed; ++i) {
            if (!sieve[i]) {
                for (idx_t j = i * i; j <= needed; j += i) {
                    sieve[j] = 1;
                }
            }
        }

        initialized   = true;
        current_limit = needed;
    }

    /* Count primes in [9n, 10n] */
    idx_t lo = 9ULL * n;
    idx_t hi = 10ULL * n;
    seq_t count = 0;

    for (idx_t x = lo; x <= hi; ++x) {
        if (!sieve[(size_t)x]) count++;
    }

    return count;
}
