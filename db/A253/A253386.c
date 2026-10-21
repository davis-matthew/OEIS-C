/*
 * A253386  Suffixes of 3991687693967 (left-truncatable prime).
 * https://oeis.org/A253386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = 3991687693967 mod 10^n (for 1 <= n <= 13)
 * Range:     u8 none, u16 n <= 1, u32 none, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Constant base number. */
    static const seq_t base = (seq_t)3991687693967ULL;

    /* Precompute powers of 10 up to 10^13 once. */
    static seq_t pow10[14];
    static bool init = false;

    if (!init) {
        pow10[0] = (seq_t)1;
        for (idx_t i = 1; i <= 13; ++i) {
            pow10[i] = pow10[i - 1] * (seq_t)10;
        }
        init = true;
    }

    if (n >= 1 && n <= 13) {
        return base % pow10[(idx_t)n];
    }

    /* For n > 13 the modulus exceeds the number, so the value stays the
       full base number. */
    return base;
}
