/*
 * A348783  Let c(i) be the number of times the digit i appears in n, for 0 <=
 *          i <= 9; then a(n) is the concatenation of c(9) c(8) ... c(1) c(0),
 *          with leading 0's omitted.
 * https://oeis.org/A348783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) is the concatenation of the digit counts of n from 9 down
 *            to 0, dropping leading zeros from the resulting decimal string.
 *            For example, a(10)=11, a(11)=20, a(100)=110
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   count_digits_seq, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/count_digits_seq.h"

seq_t a(idx_t n)
{
    unsigned char cnt[10] = {0};
    if (n == 0)
    {
        cnt[0] = 1;
    }
    else
    {
        idx_t t = n;
        while (t > 0)
        {
            unsigned d = t % 10;
            cnt[d]++;
            t /= 10;
        }
    }

    seq_t res = 0;
    for (int d = 9; d >= 0; --d)
    {
        seq_t c = (seq_t)cnt[d];
        idx_t len = count_digits_seq(c);
        seq_t shift = pow10(len);
        res = res * shift + c;
    }
    return res;
}
