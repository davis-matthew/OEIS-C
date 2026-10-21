/*
 * A277342  a(n) = b(n^2) where b(n) = n-99*floor((n-1)/99).
 * https://oeis.org/A277342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ((n*n-1) mod 99)+1 for n>0, a(0)=0
 * Range:     u8 n <= 15, u16 n <= 990, u32 n <= 990, u64 n <= 990,
 *            u128 n <= 990, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A277342.
 * a(n) = b(n^2) with b(m) = m - 99*floor((m-1)/99).
 * The implementation uses modular arithmetic with the small modulus 99,
 * and special‑cases n == 0 to return 0 as specified by the offset. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset term */

    /* Reduce n modulo 99 before squaring to avoid overflow */
    idx_t m = n % 99;
    seq_t r = (seq_t)m * (seq_t)m; /* m*m fits comfortably in seq_t */
    r %= 99;                       /* compute n^2 mod 99 */

    return (r == 0 ? (seq_t)99 : r); /* map 0 to 99, else keep r */
}
