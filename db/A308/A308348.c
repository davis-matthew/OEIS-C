/*
 * A308348  Sum of n written in each integer base from 1 to n inclusive, read
 *          as decimal numbers.
 * https://oeis.org/A308348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{b=1..n} decimal(n written in base b)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Terms: a(1)=1, a(2)=21, a(3)=132, a(4)=1232, ...  (up to n=20) */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* base 1: unary representation – n ones */
    seq_t val1 = 0;
    for (idx_t i = 0; i < n; ++i) {
        val1 = val1 * 10 + 1;
    }
    sum += val1;

    /* bases 2..n */
    for (idx_t b = 2; b <= n; ++b) {
        idx_t tmp = n;
        idx_t digits[64];
        idx_t dcnt = 0;

        /* decompose n in base b */
        while (tmp > 0) {
            digits[dcnt++] = tmp % b;
            tmp /= b;
        }

        /* if n == 0 (theoretically impossible here), force one digit */
        if (dcnt == 0) dcnt = 1;

        /* check that all digits are decimal digits (0–9) */
        bool valid = true;
        for (idx_t i = 0; i < dcnt; ++i) {
            if (digits[i] >= 10) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            /* base b representation contains non‑decimal digits – skip it */
            continue;
        }

        /* convert the digit sequence to a decimal value */
        seq_t val = 0;
        for (idx_t i = dcnt; i > 0; --i) {
            val = val * 10 + digits[i - 1];
        }
        sum += val;
    }

    return sum;
}
