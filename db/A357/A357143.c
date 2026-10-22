/*
 * A357143  a(n) is sum of the base-5 digits of n each raised to the number of
 *          digits of n in base 5.
 * https://oeis.org/A357143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑ (digitᵢ)ⁿᵈᵢᵍⁱᵗ, where digitᵢ are the base‑5 digits of n
 *            and * nᵈᵢᵍⁱᵗ is their count. Example: n = 13₁₀ = 23₅ → a(13) =
 *            2² + 3² = 13
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1u) return 0;           /* Undefined for n = 0, but return 0 safely */

    /* Extract base‑5 digits of n into a small array */
    int digits[32];                 /* Enough for 5^32 > 2^64 */
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 5);
        tmp /= 5;
    }

    seq_t sum = 0;
    /* For each digit, compute digit^len and add to sum */
    for (int i = 0; i < len; ++i) {
        seq_t base = (seq_t)digits[i];
        seq_t power = 1;
        for (int j = 0; j < len; ++j) power *= base;
        sum += power;
    }
    return sum;
}
