/*
 * A357174  a(n) = n! * Sum_{k=0..floor(n/3)} (n - 3*k)^n/(n - 3*k)!.
 * https://oeis.org/A357174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (n-3k)^n/(n-3k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t part = 1;
    idx_t m = n;
    while (1) {
        /* term = (n! / m!) * m^n = part * m^n   where part = Π_{j=m+1}^n j */
        seq_t powm = big_pow((seq_t)m, (seq_t)n);
        res += part * powm;
        if (m < 3) break;
        /* update part for next k: multiply by (m-2)*(m-1)*m  */
        part *= (seq_t)(m - 2);
        part *= (seq_t)(m - 1);
        part *= (seq_t)m;
        m -= 3;
    }
    return res;
}
