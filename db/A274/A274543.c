/*
 * A274543  Smallest number m such that every binary string of length >= m
 *          contains either an n-th power or n-th antipower.
 * https://oeis.org/A274543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 (n = 1) * a(n) = 2^{n+1} - n - 4 (n >= 2) * (derived
 *            from the recurrence a(n) = a(n-1) + 2^n - 1 for n >= 3)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is the exceptional term */
    if (n <= 1) return 1;

    /* Compute 2^{n+1} by repeated multiplication.  The loop
     * performs n+1-1 = n iterations, which is safe for all supported
     * seq_t sizes (including arbitrary-precision types).  It also
     * avoids undefined behaviour that would arise from shifting a
     * built-in integer type by more than its width. */
    seq_t pow = 2;             /* 2^1 */
    for (idx_t i = 2; i <= n + 1; ++i)
        pow *= 2;              /* multiply by 2 each step */

    /* Subtract n + 4 */
    seq_t sub = (seq_t)(n + 4);
    return pow - sub;
}
