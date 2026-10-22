/*
 * A389368  a(n) = R(A007310(n)), where R(k) = (5*k+1)/(2^r*3^s), with r and s
 *          as large as possible, that is R(k) = A065330(A016861(k)).
 * https://oeis.org/A389368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = R(A007310(n)), where R(k) = (5*k+1)/(2^r·3^s) with r,s
 *            maximal, i.e. remove all factors 2 and 3 from 5*k+1. A007310(n)
 *            lists the odd positive integers not divisible by 3
 * Range:     u8 n <= 17, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute A007310(n): the nth odd integer not a multiple of 3. */
    idx_t k;
    if ((n % 2) == 1)               /* odd index  -> 6*((n-1)/2)+1 */
        k = 6 * ((n - 1) / 2) + 1;
    else                            /* even index -> 6*(n/2)-1   */
        k = 6 * (n / 2) - 1;

    /* Compute R(k): (5*k+1) with all 2‑ and 3‑factors removed. */
    seq_t m = (seq_t)5 * (seq_t)k + 1;
    while (m % 2 == 0) m /= 2;
    while (m % 3 == 0) m /= 3;
    return m;
}
