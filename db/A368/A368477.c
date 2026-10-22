/*
 * A368477  a(n) is the number of integer tuples (b_1, b_2, ..., b_(k+1))
 *          where 0 <= b_i <= 9, such that |b_i - b_(i+1)| = d_i for all i,
 *          where (d_1, d_2, ..., d_k) is the decimal expansion of n.
 * https://oeis.org/A368477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A368477:
    a(n) = number of 10‑digit‑segment tuples (b1,…,b_{k+1}) with
    |b_i - b_{i+1}| = d_i for each i, where (d1,…,dk) is the
    decimal expansion of n (leading digit may be 0 for n=0).
    The function implements a simple dynamic programming
    over the 10 vertices {0,…,9}.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /*  ways[x] = number of partial tuples ending at digit x
        after processing the first t digits of n.  */
    seq_t ways[10];
    seq_t neww[10];
    idx_t i, d;
    idx_t ndigits = 0;
    unsigned char digits[20];  /* n fits in 64 bits → ≤20 decimal digits */

    /* ----- collect decimal digits of n (most significant first) ----- */
    if (n == 0) {
        ndigits = 1;
        digits[0] = 0;
    } else {
        idx_t tmp = n;
        while (tmp > 0) {
            digits[ndigits++] = (unsigned char)(tmp % 10);
            tmp /= 10;
        }
        /* reverse to correct order */
        for (i = 0; i < ndigits / 2; ++i) {
            unsigned char t = digits[i];
            digits[i] = digits[ndigits - 1 - i];
            digits[ndigits - 1 - i] = t;
        }
    }

    /* ----- initial state: any first digit is allowed ----- */
    for (i = 0; i < 10; ++i) ways[i] = 1;

    /* ----- process each digit constraint ----- */
    for (i = 0; i < ndigits; ++i) {
        d = digits[i];
        for (idx_t j = 0; j < 10; ++j) neww[j] = 0;

        for (idx_t x = 0; x < 10; ++x) {
            seq_t w = ways[x];
            if (!w) continue;
            if (d == 0) {
                neww[x] += w;
            } else {
                if (x + d < 10) neww[x + d] += w;
                if (x >= d)    neww[x - d] += w;
            }
        }
        for (idx_t j = 0; j < 10; ++j) ways[j] = neww[j];
    }

    /* ----- total number of tuples ----- */
    seq_t res = 0;
    for (i = 0; i < 10; ++i) res += ways[i];
    return res;
}
