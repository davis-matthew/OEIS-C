/*
 * A265561  Smallest base-3 palindrome m >= n, written in base 3.
 * https://oeis.org/A265561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base‑3 palindrome m ≥ n, written in base 3. The
 *            returned value is the base‑3 digits of m interpreted as a
 *            decimal integer
 * Range:     u8 n <= 26, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Convert n to base‑3 digits (most significant first) */
    int d[64];
    int len = 0;
    if (n == 0) {
        d[0] = 0;
        len = 1;
    } else {
        idx_t t = n;
        while (t) {
            d[len++] = t % 3;
            t /= 3;
        }
        /* reverse to most significant first */
        for (int i = 0; i < len / 2; ++i) {
            int tmp = d[i];
            d[i] = d[len - 1 - i];
            d[len - 1 - i] = tmp;
        }
    }

    /* Build the palindrome candidate based on these digits */
    int pal[64];
    for (int i = 0; i < len; ++i) {
        if (i < (len + 1) / 2) pal[i] = d[i];
        else                      pal[i] = d[len - 1 - i];
    }

    /* Compute the value of this candidate in base‑3 (as seq_t) */
    seq_t val = 0;
    for (int i = 0; i < len; ++i) {
        val = val * 3 + pal[i];
    }
    seq_t N = (seq_t)n;
    if (val >= N) {
        /* Convert palindrome digits to a decimal integer */
        seq_t res = 0;
        for (int i = 0; i < len; ++i) res = res * 10 + pal[i];
        return res;
    }

    /* Need the next palindrome of the same length.  Increment the
       left half (including middle for odd lengths). */
    int half = (len + 1) / 2;
    int left[64];
    for (int i = 0; i < half; ++i) left[i] = d[i];

    int carry = 1;
    for (int i = half - 1; i >= 0 && carry; --i) {
        int sum = left[i] + carry;
        left[i] = sum % 3;
        carry = sum / 3;
    }

    if (carry) {
        /* Overflow: produce palindrome of length len+1, e.g. 100…001 base‑3 */
        int newlen = len + 1;
        int od[64];
        od[0] = 1;
        for (int i = 1; i < newlen - 1; ++i) od[i] = 0;
        od[newlen - 1] = 1;
        seq_t res = 0;
        for (int i = 0; i < newlen; ++i) res = res * 10 + od[i];
        return res;
    }

    /* Rebuild palindrome from updated left half */
    int newpal[64];
    for (int i = 0; i < len; ++i) {
        if (i < half) newpal[i] = left[i];
        else          newpal[i] = left[len - 1 - i];
    }

    /* Return the decimal representation of the palindrome */
    seq_t res = 0;
    for (int i = 0; i < len; ++i) res = res * 10 + newpal[i];
    return res;
}
