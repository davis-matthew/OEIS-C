/*
 * A264185  Number of (n+1) X (3+1) arrays of permutations of 0..n*4+3 with
 *          each element having index change +-(.,.) 0,0 1,-1 or 2,2.
 * https://oeis.org/A264185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=8, a(2)=81, a(3)=720, a(n)=7056*10^(n-4) for n≥4
 *            (equivalently a(n)=441*2^n*5^(n-4) for n>3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special first three terms */
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)81;
    if (n == 3) return (seq_t)720;

    /* For n ≥ 4 use the closed form 7056 * 10^(n-4) */
    idx_t exp = n - 4;          /* exponent for 10 */
    seq_t base = (seq_t)10;
    seq_t pow10 = (seq_t)1;

    /* Exponentiation by squaring: O(log n) multiplications */
    while (exp)
    {
        if (exp & 1U) pow10 = pow10 * base;
        base = base * base;
        exp >>= 1U;
    }
    return (seq_t)7056 * pow10;
}
