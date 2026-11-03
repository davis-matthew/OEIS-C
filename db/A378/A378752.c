/*
 * A378752  a(n) = 2*sigma(n) - sigma(A003961(n)), where A003961 is fully
 *          multiplicative with a(prime(i)) = prime(i+1).
 * https://oeis.org/A378752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*sigma(n) - sigma(A003961(n))
 * Range:     i8 n <= 10, i16 n <= 767, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 90 terms through n = 9719 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"
#include "../utils/is_prime.h"

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
    if (n == 0) return 0;
    seq_t two_sigma = sigma_of(n) * 2;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t product = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t q = p + 1;
        while (!is_prime(q)) q++;
        seq_t qpow = ipow_u64_idx(q, e + 1);
        seq_t sigma_qe = (qpow - 1) / ((seq_t)q - 1);
        product *= sigma_qe;
    }
    return two_sigma - product;
}
