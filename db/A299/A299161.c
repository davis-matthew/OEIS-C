/*
 * A299161  In factorial base, any rational number has a terminating
 *          expansion; hence we can devise a self-inverse permutation of the
 *          rational numbers, say f, such that for any rational number q, the
 *          representations of q and of f(q) in factorial base are mirrored
 *          around the radix point and q and f(q) have th...
 * https://oeis.org/A299161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   a(n) = numerator of f(n), where f maps an integer n to the *
 *            rational number whose factorial‑base fractional digits are the
 *            same as * the factorial‑base digits of n. The fraction is
 *            reduced to lowest * terms and the numerator is returned
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _gcd128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/_gcd128.h"

#define MAX_K 64   /* maximum number of factorial‑base digits needed
                    * for a 64‑bit integer (20! < 2^64 < 21!). */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* ---------- 1. obtain factorial‑base digits of n ----------
       digits[i] holds the digit for (i+1)! (i starts at 0). */
    idx_t digits[MAX_K];
    size_t k = 0;
    idx_t t = n;
    while (t > 0) {
        idx_t base = (idx_t)(k + 2);          /* next factorial base (k+2) */
        digits[k] = t % base;
        t /= base;
        ++k;
    }

    /* ---------- 2. precompute factorials up to (k+1)! -------------
       Use 128‑bit integers to avoid overflow. */
    unsigned __int128 fac[MAX_K + 2];
    fac[0] = 1;
    for (size_t i = 1; i <= k + 1; ++i)
        fac[i] = fac[i - 1] * (unsigned __int128)i;

    unsigned __int128 denom = fac[k + 1];   /* (k+1)!  */

    /* ---------- 3. build numerator before reduction -------------
       numerator = Σ d_i * denom / (i+2)! for i = 0..k-1 */
    unsigned __int128 num = 0;
    for (size_t i = 0; i < k; ++i) {
        unsigned __int128 term = denom / fac[i + 2];
        num += (unsigned __int128)digits[i] * term;
    }

    /* ---------- 4. reduce the fraction and return numerator ----- */
    unsigned __int128 g = _gcd128(num, denom);
    unsigned __int128 reduced = num / g;

    return (seq_t)reduced;
}
