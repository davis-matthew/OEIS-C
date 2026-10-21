/*
 * A265531  Largest base-4 palindrome m <= n, written in base 10.
 * https://oeis.org/A265531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest base-4 palindrome <= n
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Extract base‑4 digits of n, most significant first. */
    uint8_t digits[64];
    size_t len = 0;

    if (n == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        idx_t tmp = n;
        uint8_t rev[64];
        size_t revlen = 0;
        while (tmp > 0) {
            rev[revlen++] = (uint8_t)(tmp & 3);  /* tmp % 4 */
            tmp >>= 2;                           /* tmp /= 4 */
        }
        for (size_t i = 0; i < revlen; ++i)
            digits[i] = rev[revlen - 1 - i];
        len = revlen;
    }

    /* Build the candidate palindrome from the most significant half. */
    size_t half = (len + 1) / 2;
    uint8_t prefix[32];
    for (size_t i = 0; i < half; ++i)
        prefix[i] = digits[i];

    seq_t val = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = (i < half) ? prefix[i] : prefix[len - 1 - i];
        val = val * (seq_t)4 + (seq_t)d;
    }

    if (val <= (seq_t)n)
        return val;

    /* Decrement the prefix (base‑4). */
    int i = (int)half - 1;
    while (i >= 0 && prefix[i] == 0) {
        prefix[i] = 3;
        --i;
    }
    if (i >= 0)
        --prefix[i];

    /* If the leading digit became zero, the answer is all 3’s of length‑1. */
    if (len > 1 && prefix[0] == 0) {
        seq_t pow4 = 1;
        for (size_t k = 0; k < len - 1; ++k)
            pow4 *= (seq_t)4;
        return pow4 - (seq_t)1;
    }

    /* Re‑build the palindrome from the decremented prefix. */
    val = 0;
    for (size_t j = 0; j < len; ++j) {
        uint8_t d = (j < half) ? prefix[j] : prefix[len - 1 - j];
        val = val * (seq_t)4 + (seq_t)d;
    }
    return val;
}
