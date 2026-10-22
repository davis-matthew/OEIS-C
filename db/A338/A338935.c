/*
 * A338935  a(n) = Sum_{d|n} (d^2 mod n).
 * https://oeis.org/A338935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (d^2 mod n)
 * Range:     u8 n <= 89, u16 n <= 4679, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _rec_divs, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/build_spf.h"
#include "../utils/_rec_divs.h"

seq_t a(idx_t n)
{
    /* Build the SPF table for numbers up to n. */
    static idx_t *spf = NULL;
    static idx_t spf_limit = 0;
    if (spf_limit < n) {
        /* allocate a new SPF array; the build routine frees the old one if needed */
        build_spf(n, &spf);
        spf_limit = n;
    }

    /* Factorise n using the SPF table. */
    idx_t tmp = n;
    idx_t primes[32];
    int    exps[32];
    int    k = 0;
    while (tmp > 1) {
        idx_t p = spf[tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[k] = p;
        exps[k]   = e;
        ++k;
    }

    /* Enumerate all divisors and sum d^2 mod n. */
    seq_t sum = 0;
    _rec_divs(0, (seq_t)1, n, primes, exps, k, &sum);
    return sum;
}
