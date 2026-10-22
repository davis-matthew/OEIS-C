/*
 * A322001  Digits of n interpreted in factorial base: a(Sum d_k*10^k) = Sum
 *          d_k*k!
 * https://oeis.org/A322001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k=0}^{m-1} d_k * (k+1)! (d_k are the decimal digits of
 *            n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;        /* accumulated sum of digit * factorial   */
    seq_t fact   = 1;        /* (1)! initially                           */
    idx_t pos    = 0;        /* position counter (0‑based)                 */
    idx_t temp   = n;        /* copy of n for digit extraction             */

    while (temp > 0) {
        unsigned int digit = temp % 10;          /* least significant decimal digit */
        result += (seq_t)digit * fact;           /* add digit * current factorial   */
        temp /= 10;                              /* remove processed digit         */
        fact *= (seq_t)(pos + 2);                /* advance factorial: (k+2)!       */
        ++pos;                                   /* next position                  */
    }

    return result;
}
