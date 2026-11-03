/*
 * A248012  a(n)=p1^(p2^(p3^(p4^...)))... where p1<p2<p3<... are the distinct
 *          prime factors of n.
 * https://oeis.org/A248012
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p1^(p2^(p3^(...))) where p1<p2<... are the distinct prime
 *            factors of n (sorted increasingly). For n with a single prime
 *            factor, a(n) = that prime. For n≥2, the function returns the
 *            described power tower
 * Range:     u8 n <= 20, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 56 terms through n = 65 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n < 2)   /* Undefined for n < 2, just return 0 */
        return 0;

    /* Factorization: store primes and exponents (exponents unused). */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t  cnt = 0;

    /* Factor n into distinct prime factors. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* If only one distinct prime factor, return it directly. */
    if (cnt == 1)
        return (seq_t)primes[0];

    /* Compute the exponent tower above the first prime. */
    seq_t exp = (seq_t)primes[cnt - 1];           /* Start with the largest prime. */
    for (int i = (int)cnt - 2; i >= 1; --i) {     /* Compute p_(i) ^ exp, from the top down. */
        exp = ipow((seq_t)primes[i], exp);
    }

    /* Finally raise the smallest prime to the computed exponent. */
    return ipow((seq_t)primes[0], exp);
}
