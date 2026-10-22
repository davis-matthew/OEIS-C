/*
 * A327412  a(n) = multinomial(3*n+2; 2, 3, 3, ..., 3) (n times '3').
 * https://oeis.org/A327412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n=0 a(n) = (9*(n-1)^3 + 36*(n-1)^2 + 47*n -
 *            27)*a(n-1)/2 for n>0 (multiplier is an unsigned 64‑bit integer)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1u;   /* a(0) = 1 */

    seq_t val = (seq_t)1u;   /* current a(k) */
    idx_t i;

    for (i = 1; i <= n; ++i)
    {
        /* Compute multiplier = 9*(i-1)^3 + 36*(i-1)^2 + 47*i - 27 */
        uint64_t im1 = i - 1u;
        uint64_t mult64 = 9u * im1 * im1 * im1
                        + 36u * im1 * im1
                        + 47u * i
                        - 27u;

        val = val * (seq_t)mult64;   /* multiply by multiplier */
        val = val / (seq_t)2u;       /* divide by 2 (exact division) */
    }
    return val;
}
