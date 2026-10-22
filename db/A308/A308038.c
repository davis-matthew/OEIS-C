/*
 * A308038  a(n) = Sum_{i=1..floor((n-1)/2)} i * (n-i)^2.
 * https://oeis.org/A308038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 - 3*(-1)^n - 2*(17+3*(-1)^n)*n^2 - 12*(1+(-1)^n)*n^3 +
 *            22*n^4)/384
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 125, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n=1,2 give 0 directly */
    if (n <= 2) {
        return 0;
    }

    /* determine (-1)^n as +1 or -1 */
    int64_t sign = (n & 1ULL) ? -1 : 1;           /* we use signed int for sign only */
    seq_t sn = (seq_t)n;                          /* base n as seq_t */

    /* compute powers of n */
    seq_t n_sq  = sn * sn;              /* n^2 */
    seq_t n_cu  = n_sq * sn;            /* n^3 */
    seq_t n_pow4 = n_cu * sn;           /* n^4 */

    /* compute numerator term by term */
    seq_t term1 = (seq_t)3 - (seq_t)3 * (seq_t)sign;

    seq_t part1 = (seq_t)(17 + 3 * sign);
    seq_t term2 = (seq_t)2 * part1 * n_sq;

    seq_t part2 = (seq_t)(1 + sign);
    seq_t term3 = (seq_t)12 * part2 * n_cu;

    seq_t term4 = (seq_t)22 * n_pow4;

    seq_t numer = term1 - term2 - term3 + term4;

    /* divide by 384 – exact division for all integer n >= 1 */
    return numer / (seq_t)384;
}
