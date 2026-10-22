/*
 * A331476  The (10^n)-th even-digit number.
 * https://oeis.org/A331476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A014263(10^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t val = pow10(n) - (seq_t)1;
    if (val == (seq_t)0) return (seq_t)0;

    /* count digits of val in base 5 */
    idx_t cnt = 0;
    seq_t tmp = val;
    while (tmp > (seq_t)0) {
        tmp /= (seq_t)5;
        ++cnt;
    }

    size_t sz = (size_t)cnt;
    unsigned char *buf = (unsigned char *)malloc(sz);
    tmp = val;
    for (idx_t i = 0; i < cnt; ++i) {
        seq_t rem = tmp % (seq_t)5;
        buf[i] = (unsigned char)rem;
        tmp /= (seq_t)5;
    }

    seq_t res = (seq_t)0;
    for (idx_t i = cnt; i-- > 0; ) {
        res = res * (seq_t)10 + (seq_t)(buf[i] * 2);
    }

    free(buf);
    return res;
}
