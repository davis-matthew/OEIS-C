/*
 * A273060  a(n) = phi(n!)/phi(n).
 * https://oeis.org/A273060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n!)/phi(n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* free() */
#include <stdint.h>          /* uint64_t */
#include "../utils/build_primes_upto.h"

/*  Static cache for prime numbers up to the largest requested n.  */
static uint64_t *prime_list = NULL;
static size_t     prime_cnt  = 0;
static idx_t      prime_limit = 0;

/*  Compute a(n) = phi(n!)/phi(n).  */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behaviour below offset is irrelevant */
        return 0;

    /*  Ensure we have all primes up to n.  */
    if (n > prime_limit) {
        if (prime_list) free(prime_list);          /* free previous allocation */
        build_primes_upto(n, &prime_list, &prime_cnt);
        prime_limit = n;
    }

    /*  Compute (n-1)!  */
    seq_t result = 1;
    for (idx_t k = 2; k <= n - 1; ++k)
        result *= (seq_t)k;

    /*  Multiply by (p-1)/p for every prime p ≤ n that does not divide n.  */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > n) break;               /* safety; primes are sorted */
        if (n % p == 0)                  /* p divides n → factor cancels out */
            continue;
        result *= (seq_t)(p - 1);
        result /= (seq_t)p;              /* exact division */
    }

    return result;
}
