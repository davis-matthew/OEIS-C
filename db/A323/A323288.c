/*
 * A323288  Largest number that can be obtained from the "Choix de Bruxelles",
 *          version 2 (A323460) operation applied to n.
 * https://oeis.org/A323288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 6564, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function computes the term a(n) of OEIS A323288.
   For a given positive integer n, its decimal representation is
   examined.  For every contiguous block of digits we form the number
   B represented by that block, double it to 2*B and replace the
   original block in the decimal string by the decimal form of 2*B.
   The resulting integer is evaluated.  a(n) is the maximum of all
   such results.  The implementation runs in O(L²) with L = number
   of decimal digits of n (≤ 20 for 64‑bit n) and works for all
   unsigned integer types seq_t and any size of idx_t (uint64_t).
   The function uses only integer arithmetic and is fully compliant
   with C11 and C++17. 
*/

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return 0; /* not used, as offset is 1 */
    }

    /* extract decimal digits (least significant first) */
    uint8_t rev[65];
    int rev_len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        rev[rev_len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    if (rev_len == 0) rev_len = 1; /* for n == 0, though not needed */

    /* rearrange to most significant first */
    int len = rev_len;
    uint8_t d[65];
    for (int i = 0; i < len; ++i) {
        d[i] = rev[len - 1 - i];
    }

    /* pre‑compute powers of 10 up to len + 1 */
    seq_t pow10[65];
    pow10[0] = 1;
    for (int i = 1; i <= len + 1; ++i) {
        pow10[i] = pow10[i - 1] * 10;
    }

    /* prefix values: prefix[i] = integer represented by d[0..i-1] */
    seq_t prefix[65];
    prefix[0] = 0;
    for (int i = 1; i <= len; ++i) {
        prefix[i] = prefix[i - 1] * 10 + d[i - 1];
    }

    /* suffix values: suffix[i] = integer represented by d[i..len-1] */
    seq_t suffix[66];
    suffix[len] = 0;
    for (int i = len - 1; i >= 0; --i) {
        suffix[i] = (seq_t)d[i] * pow10[len - i - 1] + suffix[i + 1];
    }

    /* initial best is doubling the whole number */
    seq_t best = (seq_t)n * 2;

    /* examine all contiguous blocks */
    for (int i = 0; i < len; ++i) {
        seq_t B = 0;
        for (int j = i; j < len; ++j) {
            B = B * 10 + d[j];
            seq_t doubled = B * 2;

            /* compute number of digits of doubled */
            int len2B = 1;
            seq_t t = doubled;
            while (t >= pow10[len2B]) {
                ++len2B;
            }

            int suffix_len = len - j - 1;
            int exponent = len2B + suffix_len;

            seq_t value = prefix[i] * pow10[exponent] + doubled * pow10[suffix_len] + suffix[j + 1];
            if (value > best) {
                best = value;
            }
        }
    }

    return best;
}
