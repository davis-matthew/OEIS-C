/*
 * A353825  Inverse permutation to A353824.
 * https://oeis.org/A353825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = inverse of A353824: with nonzero ternary digits e_k of n
 *            and p_0 = 1, the k-th nonzero digit of a(n) is e_k*e_{k-1} mod 3
 *            (zeros stay in place)
 * Range:     u8 n <= 255, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Validated: standard, pass, 84 terms through n = 5909 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    uint8_t digits[64];
    int count = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[count++] = (uint8_t)(tmp % 3);
        tmp /= 3;
    }

    seq_t result = 0;
    int prev = 1;
    for (int i = count - 1; i >= 0; --i) {
        int e = digits[i];
        int d = 0;
        if (e != 0) {
            d = (e * prev) % 3;
            prev = e;
        }
        result = result * 3 + (seq_t)d;
    }
    return result;
}
