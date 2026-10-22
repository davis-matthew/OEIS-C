/*
 * A338768  Sum of the remainders (p*q mod n) with p,q prime, p + q = n and p
 *          <= q.
 * https://oeis.org/A338768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p+q=n, p,q prime, p<=q} (p*q mod n)
 * Range:     u8 n <= 33, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   is_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_primes.h"
#include "../utils/is_prime.h"

static uint64_t *global_primes = NULL;
static size_t       global_primes_cnt = 0;
static idx_t        global_primes_limit = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* offset is 1, but guard against bad input */

    /* Make sure we have all primes up to n */
    if (n > global_primes_limit) {
        if (global_primes)
            free(global_primes);
        size_t cnt;
        sieve_primes(n, &global_primes, &cnt);
        global_primes_cnt   = cnt;
        global_primes_limit = n;
    }

    seq_t sum = 0;
    idx_t half = n / 2;   /* largest p to consider (p <= n/2) */

    for (size_t i = 0; i < global_primes_cnt; ++i) {
        uint64_t p = global_primes[i];
        if (p > (uint64_t)half) break;

        uint64_t q = n - p;
        if (p > q) break;           /* enforce p <= q */

        if (!is_prime((idx_t)q))
            continue;               /* q must be prime */

        seq_t prod = (seq_t)p * (seq_t)q;
        seq_t rem  = prod % (seq_t)n;
        sum += rem;
    }
    return sum;
}
