/*
 * A361832  For any number k >= 0, let T_k be the triangle whose base
 *          corresponds to the ternary expansion of k (without leading zeros)
 *          and other values, say t above u and v, satisfy t = (-u-v) mod 3;
 *          the ternary expansion of a(n) corresponds to the left border of
 *          T_n (the most significant digit being at the...
 * https://oeis.org/A361832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) built from the base‑3 digits of n by constructing a
 *            triangle whose bottom row is the ternary expansion of n and
 *            whose interior elements t satisfy t = (–u – v) mod 3. The base‑3
 *            expansion of a(n) is the left border of that triangle (most
 *            significant digit at the bottom)
 * Range:     u8 n <= 242, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* 1. Obtain the base‑3 digits of n (least significant first). */
    uint8_t tmp[64];
    size_t len = 0;
    idx_t t = n;
    while (t > 0) {
        tmp[len++] = (uint8_t)(t % 3);
        t /= 3;
    }

    /* 2. Reverse to get most significant first. */
    uint8_t base[64];
    for (size_t i = 0; i < len; ++i) {
        base[i] = tmp[len - 1 - i];
    }

    /* 3. Build the triangle, keeping only the current row. */
    uint8_t row[64];
    for (size_t i = 0; i < len; ++i) row[i] = base[i];

    /* 4. Record the left border digits of a(n). */
    uint8_t a_digits[64];
    a_digits[0] = row[0];
    size_t cur_len = len;
    for (size_t step = 1; step < len; ++step) {
        size_t next_len = cur_len - 1;
        for (size_t i = 0; i < next_len; ++i) {
            int sum = row[i] + row[i + 1];
            int rem = sum % 3;          /* rem in {0,1,2} */
            int val = (3 - rem) % 3;    /* t = (-sum) mod 3 */
            row[i] = (uint8_t)val;
        }
        a_digits[step] = row[0];
        cur_len = next_len;
    }

    /* 5. Convert the collected base‑3 digits to the final value. */
    seq_t res = 0;
    for (size_t i = 0; i < len; ++i) {
        res = res * 3 + a_digits[i];
    }
    return res;
}
