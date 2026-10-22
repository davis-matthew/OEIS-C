/*
 * A328478  Divide n by the largest primorial that divides it and repeat until
 *          a fixed point is reached; a(n) is the fixed point.
 * https://oeis.org/A328478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 9229, u32 n <= 9229, u64 n <= 9229,
 *            u128 n <= 9229, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS sequence A328478.
 * It repeatedly divides the current value by the largest primorial that
 * divides it until the value no longer changes.  The resulting value is
 * returned as the fixed point a(n). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazily initialize a list of small primes (first 300 primes are plenty). */
    static bool primes_initialized = false;
    static uint64_t primes[300];
    static size_t prime_cnt = 0;

    if (!primes_initialized) {
        /* generate primes up to a limit sufficient to get the first ~300 primes */
        const size_t limit = 2000;                  /* up to prime 1733 */
        bool *sieve = (bool *)malloc(limit + 1);
        if (!sieve) return (seq_t)0; /* fallback if memory allocation fails */

        for (size_t i = 0; i <= limit; ++i) sieve[i] = true;
        sieve[0] = sieve[1] = false;
        for (size_t p = 2; p * p <= limit; ++p) {
            if (sieve[p]) {
                for (size_t k = p * p; k <= limit; k += p)
                    sieve[k] = false;
            }
        }
        for (size_t i = 2; i <= limit; ++i) {
            if (sieve[i]) {
                if (prime_cnt < 300)
                    primes[prime_cnt++] = (uint64_t)i;
            }
        }
        free(sieve);
        primes_initialized = true;
    }

    if (n == 0) return (seq_t)0; /* Offset is 1; behaviour below is undefined */
    seq_t cur = (seq_t)n;        /* current value to reduce */
    while (1) {
        seq_t largest = (seq_t)1;
        seq_t primorial = (seq_t)1;

        for (size_t i = 0; i < prime_cnt; ++i) {
            primorial = primorial * (seq_t)primes[i];
            if (primorial > cur) break;          /* further primorials exceed cur */
            if (cur % primorial == (seq_t)0)     /* primorial divides cur */
                largest = primorial;
        }

        if (largest == (seq_t)1) break;           /* no further division possible */
        cur = cur / largest;
    }
    return cur;
}
