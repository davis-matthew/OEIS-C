/*
 * A265522  a(n) = largest base-8 palindrome m <= n such that every base-8
 *          digit of m is <= the corresponding digit of n; m is written in
 *          base 8.
 * https://oeis.org/A265522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   largest base‑8 palindrome m ≤ n such that every base‑8 digit of
 *            m * is ≤ the corresponding digit of n when both are written in
 *            base 8; * the returned value consists of m’s base‑8 digits
 *            interpreted as decimal. * This implementation enumerates all
 *            palindromes of each length ≤ |n|₈ * and keeps the...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Convert n to an array of its octal digits (most significant first). */
    int L = 0;
    uint64_t tmp = (uint64_t)n;          /* n fits in 64 bits for the conversion */
    do {
        tmp /= 8;
        ++L;
    } while (tmp);
    unsigned char digit_n[64];            /* enough for all possible lengths */
    tmp = (uint64_t)n;
    for (int i = L - 1; i >= 0; --i) {
        digit_n[i] = tmp % 8;
        tmp /= 8;
    }

    seq_t n_val   = (seq_t)n;            /* numeric value of n for comparison */
    seq_t best_oct = 0;                  /* best value in base‑8 (numeric)   */
    seq_t best_dec = 0;                  /* best value as decimal digits     */

    /* Enumerate palindromes of length 1 … L. */
    for (int len = 1; len <= L; ++len) {
        const int half_len = (len + 1) >> 1;     /* ceil(len/2) */
        const int limit    = 1 << (3 * half_len);/* 8^half_len  */

        for (int num = 0; num < limit; ++num) {
            /* Build the half digits (most significant first). */
            unsigned char half[8];
            int work = num;
            for (int i = half_len - 1; i >= 0; --i) {
                half[i] = work % 8;
                work   /= 8;
            }

            /* No leading zero allowed for canonical representation. */
            if (len > 1 && half[0] == 0)
                continue;

            /* Construct full palindrome digits. */
            unsigned char pal[64];
            for (int i = 0; i < half_len; ++i)
                pal[i] = half[i];
            for (int i = half_len; i < len; ++i)
                pal[i] = pal[len - 1 - i];

            /* Check the digitwise constraint against n's digits. */
            int offset = L - len;
            int ok = 1;
            for (int i = 0; i < len; ++i) {
                if (pal[i] > digit_n[offset + i]) {
                    ok = 0;
                    break;
                }
            }
            if (!ok)
                continue;

            /* Compute numeric value in base 8 and decimal output. */
            seq_t val_oct = 0;
            seq_t val_dec = 0;
            for (int i = 0; i < len; ++i) {
                val_oct = val_oct * 8 + pal[i];
                val_dec = val_dec * 10 + pal[i];
            }
            if (val_oct > n_val)
                continue;                /* ensure m ≤ n */

            if (val_oct > best_oct) {
                best_oct = val_oct;
                best_dec = val_dec;
            }
        }
    }
    return best_dec;
}
