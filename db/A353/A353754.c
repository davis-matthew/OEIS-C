/*
 * A353754  Greatest common divisor of phi(sigma(n)) and Product_{p^e||n}
 *          phi(sigma(p^e)), where n = Product_{p^e||n}, with each p^e the
 *          maximal power of prime p that divides n.
 * https://oeis.org/A353754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( phi(σ(n)), Π_{p^e||n} φ(σ(p^e)) )
 * Range:     u8 n <= 16, u16 n <= 2902, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 102 terms through n = 9809 (2026-11-01)
 * Helpers:   factor_u64, gcd, ipow_u64_idx, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where σ is the sum-of-divisors function and φ is Euler's totient.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/ipow_u64_idx.h"
#include "../utils/totient_seq.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    /* Special case n = 1 */
    if (n == 1) {
        return (seq_t)1;                /* σ(1)=1, φ(1)=1, product over empty set = 1 -> gcd(1,1)=1 */
    }

    /* Factor n into prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute product over prime powers */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* σ(p^e) = (p^{e+1} - 1) / (p - 1) */
        seq_t pe_pow = ipow_u64_idx(p, e + 1);               /* p^{e+1} */
        seq_t sigma_pe = (pe_pow - ((seq_t)1)) / ((seq_t)(p - 1));

        /* φ(σ(p^e)) */
        seq_t phi_sigma_pe = totient_seq(sigma_pe);

        prod = prod * phi_sigma_pe;
    }

    /* Compute φ(σ(n)) */
    seq_t sigma_n = sigma_of(n);
    seq_t phi_sigma_n = totient_seq(sigma_n);

    /* Return gcd */
    return gcd(phi_sigma_n, prod);
}
