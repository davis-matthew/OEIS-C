/*
 * A371635  For any number k >= 0, let T_k be the triangle with values in {-1,
 *          0, +1} whose base corresponds to the balanced ternary expansion of
 *          k (without leading zeros) and other values, say t above u and v,
 *          satisfy t+u+v = 0 mod 3; the balanced ternary expansion of a(n)
 *          corresponds to the left border of...
 * https://oeis.org/A371635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the integer whose balanced‑ternary digits equal the left
 *            border * of T_n, where T_n has base row equal to the
 *            balanced‑ternary digits of n and * every other entry t satisfies
 *            (t + u + v) ≡ 0 (mod 3)
 * Range:     u8 n <= 121, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Balanced ternary digits of n, most significant first.  We use a small
       fixed‑size buffer because n fits in 64 bits (max 41 digits). */
    enum { MAX_DIGITS = 70 };
    int8_t base[MAX_DIGITS];
    int L;

    if (n == 0) {
        base[0] = 0;
        L = 1;
    } else {
        int8_t rev[MAX_DIGITS];
        int r = 0;
        idx_t tmp = n;
        while (tmp > 0) {
            int rem = tmp % 3;          /* 0,1,2 */
            tmp /= 3;
            if (rem == 2) {             /* 2 -> –1 with carry */
                rem = -1;
                tmp += 1;
            }
            rev[r++] = (int8_t)rem;
        }
        L = r;
        for (int i = 0; i < r; ++i) {
            base[i] = rev[r - 1 - i];
        }
    }

    /* current row of the triangle */
    int8_t cur[MAX_DIGITS];
    for (int i = 0; i < L; ++i) cur[i] = base[i];
    int cur_len = L;

    seq_t result = 0;

    for (int step = 0; step < L; ++step) {
        int digit = cur[0];                /* leftmost element of current row */

        result = result * 3;
        if (digit >= 0) {
            result = result + (seq_t)digit;
        } else {
            result = result - (seq_t)(-digit);
        }

        if (cur_len == 1) break;

        int next[MAX_DIGITS];
        int next_len = cur_len - 1;
        for (int j = 0; j < next_len; ++j) {
            int sum = cur[j] + cur[j + 1];       /* -2 .. 2 */
            int neg = -sum;
            int res = neg % 3;
            if (res < 0) res += 3;              /* 0 .. 2 */
            if (res == 0)       next[j] = 0;
            else if (res == 1)  next[j] = 1;
            else                next[j] = -1;   /* res == 2 */
        }
        for (int j = 0; j < next_len; ++j) cur[j] = next[j];
        cur_len = next_len;
    }

    return result;
}
