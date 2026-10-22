/*
 * A383783  a(n) = Sum_{k=1..2^n} mu(k) * (floor(2^n/k)^4 -
 *          floor((2^n-1)/k)^4).
 * https://oeis.org/A383783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..2^n} mu(k) * (floor(2^n/k)^4 -
 *            floor((2^n-1)/k)^4) Simplification for powers of two: a(0) = 1
 *            a(n) = (2^n)^4 - (2^n-1)^4 - [(2^(n-1))^4 - (2^(n-1)-1)^4] for n
 *            ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   pow_int, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"   /* pow_int(seq_t base, idx_t exp)   */
#include "../utils/pow_seq.h"   /* pow_seq(seq_t a, int k)          */

seq_t a(idx_t n)
{
    /* M = 2^n */
    seq_t M      = pow_int((seq_t)2, n);
    seq_t Mminus = M - (seq_t)1;

    seq_t M4        = pow_seq(M, 4);
    seq_t Mminus4   = pow_seq(Mminus, 4);
    seq_t diff1 = M4 - Mminus4;          /* (2^n)^4 - (2^n-1)^4 */

    if (n == 0) return diff1;           /* a(0) = 1 */

    /* M2 = 2^(n-1) */
    seq_t M2      = pow_int((seq_t)2, n - 1);
    seq_t M2minus = M2 - (seq_t)1;

    seq_t M22        = pow_seq(M2, 4);
    seq_t M2minus4   = pow_seq(M2minus, 4);
    seq_t diff2 = M22 - M2minus4;       /* (2^(n-1))^4 - (2^(n-1)-1)^4 */

    return diff1 - diff2;               /* a(n) for n ≥ 1 */
}
