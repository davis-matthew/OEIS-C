/*
 * A356623  Number of ways to tile a hexagonal strip made up of 4*n+2
 *          equilateral triangles, using triangles and diamonds.
 * https://oeis.org/A356623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A356623
   Formula: a(n) = 9*a(n-1) - 7*a(n-2) + a(n-3)
   Initial values: a(0)=2, a(1)=18, a(2)=148
   The function computes the recurrence iteratively, avoiding any
   dynamic allocation or type‑dependent optimizations.  It works for
   any seq_t that supports the usual arithmetic operators and for
   any idx_t that can hold the loop index.  No memoisation is
   required; the cost is O(n) time and O(1) extra space. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)148;

    seq_t a0 = (seq_t)2;   /* a(n-3) */
    seq_t a1 = (seq_t)18;  /* a(n-2) */
    seq_t a2 = (seq_t)148; /* a(n-1) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)9 * a2 - (seq_t)7 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
