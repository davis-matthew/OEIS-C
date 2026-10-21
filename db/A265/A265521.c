/*
 * A265521  a(n) = largest base-8 palindrome m <= n such that every base-8
 *          digit of m is <= the corresponding digit of n; m is written in
 *          base 10.
 * https://oeis.org/A265521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest base‑8 palindrome m <= n such that for each
 *            position the digit of m is <= the digit of n (aligning from the
 *            least‑significant side). m is returned in base 10
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert n to base‑8 digits (most significant first) */
    uint8_t n_digs[32];
    size_t n_len = 0;
    seq_t tmp = (seq_t)n;
    if (tmp == 0) {
        n_digs[0] = 0;
        n_len = 1;
    } else {
        while (tmp != 0) {
            n_digs[n_len++] = (uint8_t)(tmp % 8);
            tmp /= 8;
        }
        /* reverse to get most significant first */
        for (size_t i = 0; i < n_len / 2; ++i) {
            uint8_t swap = n_digs[i];
            n_digs[i] = n_digs[n_len - 1 - i];
            n_digs[n_len - 1 - i] = swap;
        }
    }

    seq_t best = 0;

    for (size_t L = n_len; ; --L) {
        size_t half = (L + 1) / 2;
        unsigned long long count = 1;
        for (size_t i = 0; i < half; ++i) count *= 8ULL;   /* 8^half */

        for (unsigned long long k = 0; k < count; ++k) {
            uint8_t half_digits[32];
            unsigned long long t = k;
            for (size_t i = half; i-- > 0; ) {
                half_digits[i] = (uint8_t)(t % 8);
                t /= 8;
            }

            uint8_t pal[32];
            for (size_t i = 0; i < L; ++i) {
                if (i < half)
                    pal[i] = half_digits[i];
                else
                    pal[i] = pal[L - 1 - i];
            }

            /* reject palindromes with leading 0 for length > 1  */
            if (L > 1 && pal[0] == 0)
                continue;

            /* digit‑wise <= with n (align from least significant side) */
            bool ok = true;
            size_t start = n_len - L;   /* index in n_digs that aligns with pal[0] */
            for (size_t i = 0; i < L; ++i) {
                if (pal[i] > n_digs[start + i]) { ok = false; break; }
            }
            if (!ok)
                continue;

            /* compute decimal value of this palindrome */
            seq_t val = 0;
            for (size_t i = 0; i < L; ++i)
                val = val * 8 + pal[i];

            if (val > best)
                best = val;
        }

        if (L == 1) break;
    }

    return best;
}
