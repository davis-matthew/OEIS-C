/*
 * A387879  Number of prime factors with multiplicity (bigomega) of the least
 *          common multiple of the prime indices of n.
 * https://oeis.org/A387879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of prime factors (with multiplicity) of lcm of the
 *            prime indices of n (OEIS A387879)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"

/* Return A387879(n). */
seq_t a(idx_t n)
{
    if (n < 2)          /* a(1)=0, a(0) undefined but handled as 0 */
        return 0;

    /* Build a list of all primes up to n (inclusive). */
    uint64_t *primes = NULL;
    size_t primecnt = 0;
    build_primes_upto(n, &primes, &primecnt);

    /* Factor n into its distinct prime factors. */
    uint64_t npr[128], nexp[128];
    size_t nfactcnt = 0;
    factor_u64(n, npr, nexp, &nfactcnt);

    /* Map to store the maximum exponent for each prime appearing in the
       factorisation of the lcm of the indices.  Indices are <= n. */
    int maxExp[10001] = {0};   /* 0 .. n inclusive */

    /* For each prime divisor of n, get its index and factorise that index. */
    for (size_t fi = 0; fi < nfactcnt; ++fi) {
        uint64_t p = npr[fi];        /* prime divisor of n */

        /* Find the index of p among the primes array. */
        size_t idx = 1;             /* indices are 1‑based */
        size_t lo = 0, hi = primecnt - 1;
        while (lo <= hi) {
            size_t mid = lo + (hi - lo) / 2;
            if (primes[mid] == p) {
                idx = mid + 1;
                break;
            } else if (primes[mid] < p)
                lo = mid + 1;
            else
                hi = mid - 1;
        }

        /* Factorise the index value idx (it fits in idx_t). */
        uint64_t ip[32], iex[32];
        size_t ic = 0;
        factor_u64(idx, ip, iex, &ic);

        /* Update the maximum exponent for each prime factor of idx. */
        for (size_t ii = 0; ii < ic; ++ii)
            if (iex[ii] > maxExp[ip[ii]])
                maxExp[ip[ii]] = iex[ii];
    }

    /* Sum the stored maximum exponents to get bigomega of the lcm. */
    seq_t sum = 0;
    for (size_t p = 2; p <= n; ++p)
        if (maxExp[p])
            sum += maxExp[p];

    /* Clean up the primes array allocated by build_primes_upto. */
    free(primes);

    return sum;
}
