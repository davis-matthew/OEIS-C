/*
 * A280895  For any n>=0, least prime k = p^d-2*n, where p is a prime and d is
 *          its number of digits.
 * https://oeis.org/A280895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least prime k such that k = p^d - 2*n, where p is a prime
 *            and d is the number of digits of p
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, digit_count, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_count.h"
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

/* static cache for primes up to a limit */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_limit = 0;

/* Ensure we have a prime list covering at least up to prime_limit.
   If not allocated yet, build it once. */
static void ensure_prime_list(void)
{
    if (prime_list == NULL) {
        /* start with a moderate limit; it will be increased if needed */
        prime_limit = 2000000;          /* 2 million */
        build_primes_upto(prime_limit, &prime_list, &prime_count);
    }
}

seq_t a(idx_t n)
{
    /* coefficients in seq_t context */
    const seq_t two = (seq_t)2;

    ensure_prime_list();

    for (;;) { /* outer loop to allow limit extension if needed */
        for (size_t i = 0; i < prime_count; ++i) {
            const uint64_t p = prime_list[i];
            unsigned d = (unsigned)digit_count(p);            /* d is small */
            seq_t power = big_pow((seq_t)p, (seq_t)d);         /* p^d */
            seq_t two_n = two * (seq_t)n;                      /* 2*n */

            if (power <= two_n) continue;                     /* would underflow */

            seq_t candidate = power - two_n;
            if (candidate <= 1) continue;                     /* not prime */

            if (is_prime_big(candidate)) {
                return candidate;                             /* found the least prime */
            }
        }

        /* If we reach here, all primes up to the current limit failed.
           Double the limit and rebuild the prime list. */
        size_t old_limit = prime_limit;
        prime_limit *= 2;  /* double the search limit */
        uint64_t *new_list = NULL;
        size_t new_count = 0;

        build_primes_upto(prime_limit, &new_list, &new_count);
        /* free old list */
        free(prime_list);
        prime_list = new_list;
        prime_count = new_count;

        /* start scanning from the beginning again */
        (void)old_limit; /* unused; silence compiler warning */
    }
}
