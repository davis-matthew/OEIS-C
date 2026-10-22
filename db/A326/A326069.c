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
 * Range:     u8 n <= 209, u16 n <= 9982, u32 n <= 9982, u64 n <= 9982,
 *            u128 n <= 9982, big any n
 * Helpers:   gcd, sigma_of, smallest_prime_factor_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/smallest_prime_factor_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not used in the OEIS sequence */

    seq_t sigma_n = sigma_of(n);          /* σ(n) */
    uint64_t sf = smallest_prime_factor_idx(n);    /* smallest prime factor */
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
