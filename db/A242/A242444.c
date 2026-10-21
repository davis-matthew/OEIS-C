/*
 * A242444  Number of squares k^2 < n with k^2*p == 1 (mod n) for some prime p
 *          < n.
 * https://oeis.org/A242444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of squares k^2 < n such that k^2·p ≡ 1 (mod n) for
 *            some prime p < n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _modinv_idx, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/_modinv_idx.h"

/* Greatest common divisor for idx_t values. */
static idx_t _gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    seq_t count = 0;
    for (idx_t k = 1; (unsigned __int128)k * k < n; ++k) {
        idx_t s = (idx_t)((unsigned __int128)k * k);

        /* s must be coprime with n */
        if (_gcd_idx(s, n) != 1) continue;

        idx_t inv = _modinv_idx(s, n);
        if (inv == 0) continue;

        /* inv is in [0, n-1]; need it to be a prime < n */
        if (inv < n && is_prime(inv)) ++count;
    }
    return count;
}
