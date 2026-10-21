/*
 * A268599  Expansion of 2*x^6*(4-10*x+7*x^2)/(1-2*x)^5.
 * https://oeis.org/A268599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0, for n ≤ 5 * a(n) = 2^(n-10) * (n-5) * (n-4) * (n^2 +
 *            3*n + 10), for n ≥ 6 * * Note that for 6 ≤ n ≤ 9 the power of
 *            two is negative, so the result is a * division by 2^(10-n). The
 *            product is always divisible by that power of * two, so the shift
 *            operation yields an integer value without l...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 51, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n) is zero for n = 0 .. 5 */
    if (n <= 5)
        return 0;

    /* Convert index to seq_t only once */
    seq_t N = (seq_t)n;

    /* Compute the factors */
    seq_t t1 = N - (seq_t)5;     /* (n-5) */
    seq_t t2 = N - (seq_t)4;     /* (n-4) */
    seq_t t3 = N * N + (seq_t)3 * N + (seq_t)10;  /* n^2 + 3*n + 10 */

    /* Multiply the factors */
    seq_t prod = t1 * t2;
    prod = prod * t3;

    /* Adjust for the power of two.
     * For n ≥ 10, multiply by 2^(n-10) via left shift.
     * For 6 ≤ n ≤ 9, divide by 2^(10-n) via right shift. */
    if (n >= 10) {
        prod <<= (n - 10);
    } else {                /* 6 ≤ n ≤ 9 */
        prod >>= (10 - n);
    }

    return prod;
}
