/*
 * A264800  Nearly-Fibonacci sequence.
 * https://oeis.org/A264800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n) for 1 <= n <= 3 and n >= 5; a(4) = 4. F is the
 *            Fibonacci sequence
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the exceptional low‑index values. */
    if (n == 1 || n == 2) return (seq_t)1;
    if (n == 3)           return (seq_t)2;
    if (n == 4)           return (seq_t)4;

    /* Compute the Fibonacci number for n >= 5. */
    seq_t f1 = (seq_t)1;  /* F(1) */
    seq_t f2 = (seq_t)1;  /* F(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t f3 = f1 + f2;  /* F(i) = F(i-1)+F(i-2) */
        f1 = f2;
        f2 = f3;
    }
    return f2;  /* f2 now holds F(n) */
}
