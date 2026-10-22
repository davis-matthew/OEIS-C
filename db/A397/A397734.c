/*
 * A397734  a(n) is the number of functions f:[n] -> [n] such that f(i) =
 *          f(f(i)) = f(n+1-i) for all i in [n].
 * https://oeis.org/A397734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k) = Σ_{j=0..k} C(k,j)·2^j·j^(k-j) a(2k+1) = Σ_{j=0..k}
 *            C(k,j)·2^j·(j^(k-j+1)+(j+1)^(k-j))
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, pow2, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return 1;

    idx_t k = n >> 1;          /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t j = 0; j <= k; ++j)
    {
        seq_t bin = binomial(k, j);          /* C(k,j)   */
        seq_t two = pow2(j);                 /* 2^j      */

        seq_t inner;
        if ((n & 1ULL) == 0)        /* even n -> 2k  */
        {
            inner = seq_pow((seq_t)j, k - j);   /* j^(k-j)  */
        }
        else                        /* odd n  -> 2k+1 */
        {
            seq_t part1 = seq_pow((seq_t)j,     k - j + 1); /* j^(k-j+1) */
            seq_t part2 = seq_pow((seq_t)(j + 1), k - j);   /* (j+1)^(k-j) */
            inner = part1 + part2;
        }

        sum += bin * two * inner;
    }
    return sum;
}
