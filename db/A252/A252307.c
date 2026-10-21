/*
 * A252307  Number of (2+2)X(n+2) 0..3 arrays with every 3X3 subblock row and
 *          diagonal sum equal to 0 1 3 6 or 7 and every 3X3 column and
 *          antidiagonal sum not equal to 0 1 3 6 or 7.
 * https://oeis.org/A252307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-3) - 42*a(n-6) + 64*a(n-9) - 32*a(n-12) for n>21
 *            a(1) = 1048, a(2) = 1031, a(3) = 1370, ... (first 21 terms
 *            listed below)
 * Range:     u8 none, u16 n <= 11, u32 n <= 35, u64 n <= 82, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static const seq_t init[21] = {
        1048, 1031, 1370, 1992, 3171, 5056, 7625, 12412, 19904, 30526,
        49974, 80432, 123996, 203444, 327856, 506772, 832064, 1341188,
        2076284, 3409168, 5493844
    };

    if (n <= 21)
        return init[n - 1];

    seq_t buf[12];
    for (idx_t i = 0; i < 12; ++i)
        buf[i] = init[i];            /* buf[0] = a(1), buf[11] = a(12) */

    for (idx_t k = 13; k <= n; ++k) {
        seq_t res;
        if (k <= 21) {
            res = init[k - 1];
        } else {
            /* pos = 11*a(k-3) + 64*a(k-9) */
            seq_t pos = buf[(k - 4) % 12];          /* a(k-3) */
            pos *= 11;
            seq_t tmp = buf[(k - 10) % 12];        /* a(k-9) */
            tmp *= 64;
            pos += tmp;

            /* neg = 42*a(k-6) + 32*a(k-12) */
            seq_t neg = buf[(k - 7) % 12];          /* a(k-6) */
            neg *= 42;
            tmp = buf[(k - 13) % 12];              /* a(k-12) */
            tmp *= 32;
            neg += tmp;

            res = pos - neg;     /* pos >= neg by construction */
        }
        buf[(k - 1) % 12] = res;          /* overwrite a(k-12) */
    }
    return buf[(n - 1) % 12];
}
