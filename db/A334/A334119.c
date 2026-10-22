/*
 * A334119  Total area of all triangles such that p + q = 2*n, p < q (p, q
 *          prime), with base (q - p) and height q.
 * https://oeis.org/A334119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1}^{n-1} (n-i)*(2n-i)*c(i)*c(2n-i)
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_in_list, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where c(x) = 1 if x is prime, 0 otherwise.                */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sieve_primes.h"   /* provides sieve_primes */
#include "../utils/is_prime_in_list.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* Cache primes up to the largest limit seen so far */
    static idx_t max_limit = 0;
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;

    idx_t target_limit = 2 * n;
    if (target_limit > max_limit) {
        free(primes);
        sieve_primes(target_limit, &primes, &prime_cnt);
        max_limit = target_limit;
    }

    seq_t sum = 0;
    for (size_t ii = 0; ii < prime_cnt; ++ii) {
        idx_t p = primes[ii];
        if (p >= n) break;           /* only primes < n  */
        idx_t q = 2 * n - p;
        if (!is_prime_in_list(q, primes, prime_cnt)) continue;

        /* area = (n-p)*(2n-p) */
        sum += (seq_t)(n - p) * (seq_t)q;
    }

    return sum;
}
