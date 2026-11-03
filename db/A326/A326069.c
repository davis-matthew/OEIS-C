/*
 * A326069  a(n) = gcd((sigma(n) - sigma(A032742(n))) - n, n -
 *          sigma(A032742(n))), where A032742 gives the largest proper divisor
 *          of n.
 * https://oeis.org/A326069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd((sigma(n) - sigma(A032742(n))) - n, n -
 *            sigma(A032742(n)))
 * Range:     u8 n <= 209, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9982 (2026-11-01)
 * Helpers:   gcd, smallest_prime_factor_idx_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/smallest_prime_factor_idx_to_u64.h"

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

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not used in the OEIS sequence */

    seq_t sigma_n = sigma_of(n);          /* σ(n) */
    uint64_t sf = smallest_prime_factor_idx_to_u64(n);    /* smallest prime factor */
    uint64_t lpd = n / sf;                /* largest proper divisor */
    seq_t sigma_lpd = sigma_of(lpd);      /* σ(A032742(n)) */

    seq_t a_val, b_val;
    seq_t seq_n = (seq_t)n;

    /* compute |(σ(n)-σ(lpd)) - n| */
    if (sigma_n >= sigma_lpd) {
        seq_t diff = sigma_n - sigma_lpd;
        if (diff >= seq_n)
            a_val = diff - seq_n;
        else
            a_val = seq_n - diff;
    } else {
        seq_t diff = sigma_lpd - sigma_n;
        a_val = diff + seq_n;
    }

    /* compute |n - σ(lpd)| */
    if (seq_n >= sigma_lpd)
        b_val = seq_n - sigma_lpd;
    else
        b_val = sigma_lpd - seq_n;

    return gcd(a_val, b_val);
}
