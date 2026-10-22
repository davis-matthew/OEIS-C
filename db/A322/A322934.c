/*
 * A322934  The successive approximations up to 2^n for 2-adic integer
 *          7^(1/3).
 * https://oeis.org/A322934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0, * a(n) = a(n-1) if (a(n-1)^3 - 7) mod 2^n
 *            == 0, * a(n) = a(n-1) + 2^(n-1) otherwise. * This implementation
 *            follows the recurrence directly, using only * integer arithmetic
 *            and bitwise operations. No auxiliary utilities * (pow2, big_pow,
 *            etc.) are required. The algorithm runs...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 0;          /* a(0) */

    if (n == 0)
        return x;

    /* mask = 2^i - 1  (initially for i = 1) */
    seq_t mask = 1;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* 2^i  and 2^(i-1) */
        seq_t pow2i   = mask + 1;               /* 2^i  */
        seq_t pow2im1 = (mask + 1) / 2;          /* 2^(i-1) */

        /* cube of current x */
        seq_t cube = x * x * x;

        /* (cube - 7) modulo 2^i is equivalent to (cube - 7) & mask */
        seq_t diff = cube - (seq_t)7;
        if ((diff & mask) != (seq_t)0)
            x += pow2im1;

        /* Prepare mask for next iteration: mask = 2^(i+1) - 1 */
        mask = (mask * (seq_t)2) + 1;
    }

    return x;
}
