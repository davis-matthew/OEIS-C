/*
 * A378123  a(n) = number of prime divisors of the sum of the first n odd
 *          primes.
 * https://oeis.org/A378123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime divisors of the sum of the first n odd
 *            primes
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   build_primes_upto, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/count_distinct_primes.h"

/*  Computes a(n) ; external linkage required */
seq_t a(idx_t n) {
    /* The sequence starts at n = 1; for other values return 0. */
    if (n < 1) return (seq_t)0;

    /*--- Cache of primes ------------------------------------------*/
    static uint64_t *prime_list = NULL;   /* primes starting with 2 */
    static size_t   prime_count = 0;

    /* We need at least n+1 primes to have the first n odd primes
       available (prime_list[1] = 3, the first odd prime). */
    idx_t need = (idx_t)(n + 1);
    if (prime_count < need) {
        /* Initial guess for the (n+1)-th prime; use a safe upper bound. */
        idx_t bound = (idx_t)(n + 1) * 20 + 100;
        if (bound < 100) bound = 100;

        /* Keep increasing the bound until we have enough primes. */
        while (1) {
            uint64_t *tmp;
            size_t   cnt;
            /* build_primes_upto allocates an array of all primes <= bound. */
            build_primes_upto(bound, &tmp, &cnt);
            if (prime_count != 0) free(prime_list);   /* free old list */

            prime_list  = tmp;
            prime_count = cnt;

            if (prime_count >= (size_t)need) break;
            /* Not enough primes; double the bound and try again. */
            bound *= 2;
        }
    }

    /*--- Sum of the first n odd primes --------------------------------*/
    uint64_t sum64 = 0;                     /* works for all n <= 10000   */
    for (idx_t i = 1; i <= n; ++i) {
        sum64 += prime_list[i];             /* prime_list[1] = 3             */
    }

    /*--- Count distinct prime divisors of the sum ----------------------*/
    int distinct = count_distinct_primes(sum64);

    return (seq_t)distinct;
}
