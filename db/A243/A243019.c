/*
 * A243019  Expansion of -(2*x*sqrt(1-8*x^2)-2*x) /
 *          (16*x^3+sqrt(1-8*x^2)*(4*x^2+2*x-1)-8*x^2-2*x+1).
 * https://oeis.org/A243019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0..floor(n/2)} 2^i * binomial(n,i)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t curC = 1;     /* binomial(n,0) */
    seq_t pow2 = 1;     /* 2^0 */
    idx_t k = n / 2;    /* floor(n/2) */

    for (idx_t i = 0; i <= k; ++i) {
        sum += curC * pow2;

        if (i < k) {
            curC = curC * (seq_t)(n - i) / (seq_t)(i + 1);
            pow2 = pow2 * (seq_t)2;
        }
    }
    return sum;
}
