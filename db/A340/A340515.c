/*
 * A340515  a(n) = minimal order of a group in which all groups of order <= n
 *          can be embedded.
 * https://oeis.org/A340515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ∏_{primes p ≤ n} p^{⌊log_p n⌋ + extra(p,n)} where
 *            extra(2,n) = ⌊n/4⌋ extra(p>2,n) = ⌊(n-1)/(p^2-1)⌋ (p odd prime)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   build_primes_upto, pow_int, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow_int.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    static uint64_t *primes = NULL;
    static size_t   prime_cnt = 0;
    static idx_t    prime_limit = 0;

    /* Build prime table up to n (expand as needed) */
    if (n > prime_limit) {
        if (primes) free(primes);
        build_primes_upto(n, &primes, &prime_cnt);
        prime_limit = n;
    }

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];

        /* exponent from lcm(1..n)              */
        idx_t  e_lcm = 0;
        uint64_t t = 1;
        while (t <= n / p) {
            t *= p;
            ++e_lcm;
        }

        /* extra exponent needed by non‑nilpotent groups  */
        idx_t extra;
        if (p == 2) {
            extra = n / 4;                     /* ⌊n/4⌋  */
        } else {
            uint64_t denom = (uint64_t)p * (uint64_t)p - 1;   /* p^2‑1  */
            extra = (n > 0) ? ((n - 1) / denom) : 0;         /* ⌊(n‑1)/denom⌋ */
        }

        idx_t total_exp = e_lcm + extra;

        /* multiply the prime power into result   */
        seq_t term = pow_int((seq_t)p, total_exp);
        result = seq_mul(result, term);
    }

    return result;
}
