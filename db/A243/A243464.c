/*
 * A243464  Number of ways 2 domicules can be placed on an n X n square.
 * https://oeis.org/A243464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -43 + 63*n - 4*n^2 - 24*n^3 + 8*n^4 (n > 0, a(0) = 0)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 152, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* cast n to seq_t for all arithmetic */
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;          /* n^2 */
    seq_t n3  = n2 * sn;          /* n^3 */
    seq_t n4  = n2 * n2;          /* n^4 */

    /* compute left and right parts to avoid unsigned underflow */
    seq_t left  = (seq_t)8 * n4 + (seq_t)63 * sn;          /* 8*n^4 + 63*n */
    seq_t right = (seq_t)24 * n3 + (seq_t)4 * n2 + (seq_t)43; /* 24*n^3 + 4*n^2 + 43 */

    return left - right; /* guaranteed non‑negative for n >= 1 */
}
