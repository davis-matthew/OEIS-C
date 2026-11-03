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
 * Validated: standard, pass, 83 terms through n = 9041 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A341767: replace each decimal digit d of n with
   the digital root of n^d and form the resulting digit string. */
#include <stdbool.h>

static inline seq_t digital_root_pow_mod9(unsigned n_mod9, unsigned d)
{
    /* handle n^0 case */
    if (d == 0) return 1;

    /* n mod 9 == 0 => n^d ≡ 0 (mod 9) for d>0, digital root 9 */
    if (n_mod9 == 0) return 9;

    /* n mod 9 == 1 => 1^d ≡ 1 (mod 9) */
    if (n_mod9 == 1) return 1;

    /* compute (n_mod9^d) mod 9 */
    unsigned rem = 1 % 9;
    for (unsigned i = 0; i < d; ++i)
        rem = (rem * n_mod9) % 9;

    return (rem == 0) ? 9 : rem;
}

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
