/*
 * A293239  Number of terms in the fully expanded n-th derivative of x^x.
 * https://oeis.org/A293239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n^2 + 2*n + D) / 8, where D = 8 if n mod 4 = 0 2 if n
 *            mod 4 = 1 4 if n mod 4 = 2 6 if n mod 4 = 3 This follows from
 *            the complex‑exponential representation a(n) = (5 + (-1)^n +
 *            (1-i)(-i)^n + (1+i)i^n + 2n + 4n^2)/8
 * Range:     u8 n <= 7, u16 n <= 142, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[8] = {1, 2, 4, 7, 11, 15, 21, 28};

    if (n < 8)
        return init[n];

    /* Determine D based on n mod 4 */
    unsigned int d;
    switch (n & 3U)            /* n % 4 */
    {
    case 0: d = 8; break;      /* n ≡ 0 (mod 4) */
    case 1: d = 2; break;      /* n ≡ 1 (mod 4) */
    case 2: d = 4; break;      /* n ≡ 2 (mod 4) */
    default: d = 6;            /* n ≡ 3 (mod 4) */
    }

    /* Compute (4*n^2 + 2*n + d) / 8 using type seq_t for safety */
    seq_t tn = (seq_t)n;                /* promote index to seq_t */
    seq_t num = (seq_t)4 * tn * tn +   /* 4 * n^2    */
                 (seq_t)2 * tn +      /* 2 * n      */
                 (seq_t)d;            /* + D        */

    return num / (seq_t)8;             /* divide by 8 */
}
