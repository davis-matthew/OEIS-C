/*
 * A400197  The union of the squares (A000290) and the integer 2.
 * https://oeis.org/A400197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = 0 if n = 0 a(n) = 1 if n = 1 a(n) = 2 if n = 2 a(n) =
 *            (n‑1)^2 if n ≥ 3
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the union of all perfect squares (including 0 and 1)
   with the number 2, arranged in ascending order.  For n ≥ 3 the
   (n‑1)-th square produces the term, after the initial three entries.  */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t x = (seq_t)(n - 1);
    return x * x;
}
