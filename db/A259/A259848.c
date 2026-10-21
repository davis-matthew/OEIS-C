/*
 * A259848  Suffixes of 4818372912366173 (all primes).
 * https://oeis.org/A259848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base, less
 * Formula:   a(n) = 4818372912366173 mod 10^n
 * Range:     u8 n <= 1, u16 none, u32 none, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* 10^0 = 1, so any number mod 1 is 0 */
    const seq_t base = (seq_t)4818372912366173ULL;
    /* For n ≥ 16, 10^n exceeds the constant, so the modulus is the number itself. */
    if (n >= 16)
        return base;
    seq_t pow10 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow10 *= 10;                    /* 10^n for n ≤ 15 fits in seq_t */
    return base % pow10;
}
