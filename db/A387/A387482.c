/*
 * A387482  a(n) = Sum_{k=0..floor(n/3)} 3^k * 2^(n-3*k) *
 *          binomial(k,n-3*k)^2.
 * https://oeis.org/A387482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 3^k * 2^(n-3*k) *
 *            binomial(k,n-3*k)^2
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 58, u128 n <= 114,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t m = n - 3 * k;
        if (m > k)   /* binomial(k,m) is 0 for m>k */
            continue;
        seq_t b   = binomial(k, m);   /* C(k,m) */
        seq_t term = pow3(k);         /* 3^k      */
        term *= pow2(m);              /* *2^m     */
        term *= b;                    /* *C(k,m)  */
        term *= b;                    /* *C(k,m)  */
        sum += term;
    }
    return sum;
}
