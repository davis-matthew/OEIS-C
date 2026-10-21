/*
 * A275465  a(n) = f^(n/f), where f is the smallest prime factor of n.
 * https://oeis.org/A275465
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = f^(n/f) where f is the smallest prime factor of n
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 62, u64 n <= 122,
 *            u128 n <= 242, big any n
 * Helpers:   big_pow, lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n ≥ 2; for n < 2 the sequence is undefined, we return 0. */

#include "../utils/lpf.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    idx_t f = lpf(n);          /* smallest prime factor of n */
    if (!f) f = n;             /* safety: treat n as its own factor if lpf fails */
    seq_t base = (seq_t)f;
    seq_t exp  = (seq_t)(n / f);
    return big_pow(base, exp);
}
