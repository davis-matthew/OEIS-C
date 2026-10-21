/*
 * A273375  Squares ending in digit 4.
 * https://oeis.org/A273375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10*n + (-1)^n - 5)^2 / 4
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n ≥ 1, 10*n + (-1)^n - 5 is always even, so we compute */
seq_t a(idx_t n)
{
    if (n == 0)                      /* offset is 1, but guard against 0 */
        return 0;

    seq_t sn = (seq_t)n;              /* cast index to seq_t */
    seq_t tmp;

    if (n & 1ULL)                     /* n is odd: (-1)^n = -1   */
        tmp = sn * 10 - (seq_t)1 - (seq_t)5;
    else                              /* n is even: (-1)^n = +1  */
        tmp = sn * 10 + (seq_t)1 - (seq_t)5;

    seq_t half = tmp >> 1;            /* divide by 2 – result is integer   */
    return half * half;               /* square the half to get a(n)        */
}
