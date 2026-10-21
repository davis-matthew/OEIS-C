/*
 * A255593  Convert n to base 8, move least significant digit to most
 *          significant digit and convert back to base 10.
 * https://oeis.org/A255593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = move the least significant octal digit of n to the most
 *            significant position and interpret the resulting base‑8 number
 *            in base‑10
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0) return 0;

    /* Find the highest power of 8 <= n.
       Because 8 = 2³, we can use shifts. */
    idx_t pow = 1;                /* 8⁰ */
    while (pow <= n / 8) {
        pow <<= 3;                /* multiply by 8 */
    }

    /* Least significant base‑8 digit */
    seq_t d0  = (seq_t)(n & 7);   /* n % 8 */

    /* Remaining digits (n without the least significant digit) */
    idx_t rest = n >> 3;          /* n / 8 */

    /* Rotated value: d0 becomes the most significant digit */
    return d0 * (seq_t)pow + (seq_t)rest;
}
