/*
 * A250877  T(n,k)=Number of (n+1)X(k+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(i,j) where (i,j) is the pair on the n-th
 *            anti‑diagonal * of the array of T values. The value T(i,j) is
 *            given by the * closed form * * T(i,j) =
 *            (124*j³+312*j²+260*j+72)·4^i * - (135*j³+243*j²+108*j)·3^i * +
 *            6·(9*j³-9*j)·2^i * + (5*j³-15*j²+16*j) *
 *            -------------------------------- * 18 *...
 * Range:     u8 none, u16 n <= 8, u32 n <= 76, u64 n <= 160, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1UL) result *= base;
        base *= base;
        exp >>= 1UL;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 */

    /* find the diagonal d such that d*(d-1)/2 >= n */
    idx_t d = 2;
    while ((d * (d - 1)) / 2 < n) ++d;

    /* previous count up to diagonal d-1 */
    idx_t prev = ((d - 1) * (d - 2)) / 2;

    /* position (0‑based) within diagonal d */
    idx_t pos = n - prev - 1;

    /* i = 1 + pos,  j = d - i  */
    idx_t i = pos + 1;
    idx_t j = d - i;

    seq_t k  = (seq_t)j;
    seq_t k2 = k * k;
    seq_t k3 = k2 * k;

    seq_t numer1 = (seq_t)124 * k3 + (seq_t)312 * k2 + (seq_t)260 * k + (seq_t)72;
    seq_t numer2 = (seq_t)135 * k3 + (seq_t)243 * k2 + (seq_t)108 * k;
    seq_t term3_scale = (seq_t)6 * ((seq_t)9 * k3 - (seq_t)9 * k);
    seq_t numer4 = (seq_t)5 * k3 - (seq_t)15 * k2 + (seq_t)16 * k;

    seq_t pow4n = pow_uint((seq_t)4, i);
    seq_t pow3n = pow_uint((seq_t)3, i);
    seq_t pow2n = pow_uint((seq_t)2, i);

    seq_t large = numer1 * pow4n - numer2 * pow3n + term3_scale * pow2n + numer4;
    seq_t result = large / (seq_t)18;    /* division is exact */

    return result;
}
