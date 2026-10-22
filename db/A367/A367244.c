/*
 * A367244  a(n) is the number of n-digit numbers whose difference between the
 *          largest and smallest digits is equal to 3.
 * https://oeis.org/A367244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 27*4^(n-1) - 41*3^(n-1) + 7*2^n (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility: compute base^exp using only seq_t arithmetic. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)   /* plain loop: O(exp), exp ≤ 10000 → fast enough */
    {
        result *= base;
        exp--;
    }
    return result;
}

/* Return the nth term of OEIS A367244. */
seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1. For safety return 0 if n==0. */
    if (n == 0)
        return (seq_t)0;

    /* Compute the necessary powers. */
    seq_t p4 = pow_seq(4, n - 1);   /* 4^(n-1)   */
    seq_t p3 = pow_seq(3, n - 1);   /* 3^(n-1)   */
    seq_t p2 = pow_seq(2, n);       /* 2^n       */

    /* Evaluate the linear combination. */
    seq_t term1 = 27 * p4;  /* 27 * 4^(n-1) */
    seq_t term2 = 7 * p2;   /* 7 * 2^n     */
    seq_t term3 = 41 * p3;  /* 41 * 3^(n-1) */

    /* All arithmetic is on seq_t, which is unsigned, so no overflows
       can affect the final result when it fits within seq_t. */
    return (term1 + term2) - term3;
}
