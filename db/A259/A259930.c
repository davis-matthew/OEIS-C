/*
 * A259930  Value of DIS (Decimal Integer Series) constant based on the
 *          sequence of powers of 3.
 * https://oeis.org/A259930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, base
 * Formula:   a(n) = n-th decimal digit (a(0) = first digit after the point)
 *            of sum_{k>=0} 3^k/100^A055642(3^k), computed by exact decimal
 *            column addition of the powers of 3 with carry propagation
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 79 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    int L = (int)n + 1 + 40;               /* positions 1..L, with guard digits */
    int *col = (int *)calloc((size_t)L + 2, sizeof(int));
    int *pw = (int *)calloc((size_t)L + 8, sizeof(int)); /* little endian digits of 3^k */
    int len = 1;
    pw[0] = 1;
    while (1) {
        int d = len;
        if (d + 1 > L)
            break;
        for (int i = 0; i < d; i++) {
            int pos = d + 1 + i;           /* msd first */
            if (pos > L)
                break;
            col[pos] += pw[d - 1 - i];
        }
        /* multiply by 3 */
        int carry = 0;
        for (int i = 0; i < len; i++) {
            int v = pw[i] * 3 + carry;
            pw[i] = v % 10;
            carry = v / 10;
        }
        while (carry) {
            pw[len++] = carry % 10;
            carry /= 10;
        }
    }
    int target = (int)n + 1;
    int result = 0;
    int carry = 0;
    for (int j = L; j >= 1; j--) {
        int v = col[j] + carry;
        if (j == target)
            result = v % 10;
        carry = v / 10;
    }
    free(col);
    free(pw);
    return (seq_t)result;
}
