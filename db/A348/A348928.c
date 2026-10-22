/*
 * A348928  a(n) = gcd(n, A003958(n)), where A003958 is multiplicative with
 *          a(p^e) = (p-1)^e.
 * https://oeis.org/A348928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd( n, A003958(n) ) where A003958 is multiplicative with
 *            a(p^e) = (p-1)^e. The function finds a(n) by factoring n and
 *            computing the common prime exponents with the product of (p-1)^e
 * Range:     u8 n <= 9743, u16 n <= 9953, u32 n <= 9953, u64 n <= 9953,
 *            u128 n <= 9953, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 safely */

    /* Factor n: primes_arr[i]^exps_arr[i]  (i = 0..pn-1) */
    uint64_t primes_arr[64];
    uint64_t exps_arr[64];
    size_t pn;
    factor_u64((uint64_t)n, primes_arr, exps_arr, &pn);

    /* For n = 1, A003958(1) = 1, gcd = 1 */
    if (pn == 0) return 1;

    /* Contribution of each prime of n from the (p-1)^e factors. */
    uint64_t contrib[64] = {0};

    for (size_t i = 0; i < pn; ++i) {
        uint64_t p     = primes_arr[i];
        uint64_t e_p   = exps_arr[i];
        uint64_t p_minus = p - 1;

        uint64_t pf_arr[64];
        uint64_t pf_ex[64];
        size_t pf_cnt;
        factor_u64(p_minus, pf_arr, pf_ex, &pf_cnt);

        for (size_t j = 0; j < pf_cnt; ++j) {
            uint64_t q = pf_arr[j];
            uint64_t vq = pf_ex[j];
            /* If q is one of the primes dividing n, add its contribution */
            for (size_t k = 0; k < pn; ++k) {
                if (q == primes_arr[k]) {
                    contrib[k] += e_p * vq;
                }
            }
        }
    }

    /* Build the gcd from the minimum exponent of each prime in n and in A003958(n) */
    seq_t result = 1;
    for (size_t i = 0; i < pn; ++i) {
        uint64_t exp = exps_arr[i];
        if (contrib[i] < exp) exp = contrib[i];
        /* Multiply the prime factor exp times */
        for (uint64_t e = 0; e < exp; ++e) {
            result *= (seq_t)primes_arr[i];
        }
    }
    return result;
}
