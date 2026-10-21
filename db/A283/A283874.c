/*
 * A283874  The Pierce expansion of the number Sum_{k>=1} 1/3^((2^k) - 1).
 * https://oeis.org/A283874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 2
   a(2k+1) = 3^(2^k)
   a(2k+2) = 3^(2^k) + 1  (k >= 0) */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)2;          /* a(0) */

    /* For n >= 1 we need k = floor((n-1)/2) */
    idx_t k = (n - (idx_t)1) / ((idx_t)2);

    /* Compute 3^(2^k) by repeated squaring:
       Start with 3; each loop squares the result.
       After k squarings we have 3^(2^k). */
    seq_t val = (seq_t)3;
    for (idx_t i = (idx_t)0; i < k; ++i)
        val = val * val;

    /* If n is odd (2k+1) return val, else return val+1 (2k+2). */
    return (n & (idx_t)1) ? val : (val + (seq_t)1);
}
