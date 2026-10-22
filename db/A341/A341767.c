/*
 * A341767  Replace each digit d in the decimal representation of n with the
 *          digital root of n^d.
 * https://oeis.org/A341767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digital_root_pow_mod9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A341767: replace each decimal digit d of n with
   the digital root of n^d and form the resulting digit string. */
#include <stdbool.h>
#include "../utils/digital_root_pow_mod9.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1, but guard anyway */

    /* collect decimal digits in reverse order */
    unsigned digits[20];
    uint8_t idx = 0;
    idx_t temp = n;
    while (temp > 0) {
        digits[idx++] = temp % 10;
        temp /= 10;
    }

    seq_t result = 0;
    unsigned n_mod9 = (unsigned)(n % 9);

    /* process digits from most significant to least significant */
    for (int i = idx - 1; i >= 0; --i) {
        unsigned d = digits[i];
        seq_t dr = digital_root_pow_mod9(n_mod9, d);
        result = result * 10 + dr;
    }
    return result;
}
