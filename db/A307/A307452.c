/*
 * A307452  Primes p such that the sum of the decimal digits of p^4 is also a
 *          prime.
 * https://oeis.org/A307452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 23, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   build_primes_upto, digit_sum, grow_seq, is_prime_big, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the nth prime p such that the sum of the decimal digits of p⁴ is prime. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/pow_int.h"
#include "../utils/digit_sum.h"
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Handle the offset (a(1) is the first term). */
    if (n == 0) return (seq_t)0;

    /* Static cache of qualifying primes */
    static seq_t *qual_primes = NULL;
    static size_t qual_cap   = 0;
    static size_t qual_size  = 0;

    /* Static data for prime generation */
    static uint64_t *prime_arr   = NULL;
    static size_t   prime_cnt   = 0;
    static size_t   prime_idx   = 0;      /* next prime to process */
    static idx_t    sieve_limit = 0;      /* highest limit already sieved */

    /* If we already have enough terms, return the cached one. */
    if (qual_size >= (size_t)n) return qual_primes[(size_t)n - 1];

    /* Grow the sieve until we have at least n qualifying primes. */
    idx_t target = (sieve_limit == 0) ? 1000 : sieve_limit * 2;
    while (qual_size < (size_t)n) {
        if (target <= sieve_limit + 100) target = sieve_limit + 100;

        /* Build primes up to the new target. */
        if (prime_arr != NULL) free(prime_arr);   /* free previous buffer */
        build_primes_upto(target, &prime_arr, &prime_cnt);

        /* Process newly discovered primes. */
        for (size_t i = prime_idx; i < prime_cnt; ++i) {
            uint64_t p = prime_arr[i];
            seq_t p4   = pow_int((seq_t)p, 4);
            seq_t sum  = digit_sum(p4);

            if (is_prime_big(sum)) {
                /* Grow the qualifying prime array if needed. */
                if (qual_size == qual_cap)
                    grow_seq(&qual_primes, &qual_cap, qual_size + 1);
                qual_primes[qual_size++] = (seq_t)p;
            }
        }

        /* Update bookkeeping for the next iteration. */
        prime_idx   = prime_cnt;
        sieve_limit = target;
        target      = sieve_limit * 2;
    }

    return qual_primes[(size_t)n - 1];
}
