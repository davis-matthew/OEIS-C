/*
 * A387645  a(n) = m*(m-1)^2/2, where m = ceiling(n^2/2).
 * https://oeis.org/A387645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = ceil(n^2/2) * (ceil(n^2/2)-1)^2 / 2
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A387645 for index n (1‑based). */
seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic, compute m = ceil(n^2/2). */
    seq_t m = ((seq_t)n * (seq_t)n + (seq_t)(n & 1)) / 2;
    seq_t diff = m - 1;
    return m * diff * diff / 2;
}
