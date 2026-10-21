/*
 * A296909  Partial sums of A296368.
 * https://oeis.org/A296909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296909: Partial sums of A296368
   Formula: a(0) = 1
   a(n) = 2*n^2 + 2*n                           for n=1,2,3
   a(n) = 2*n^2 + 2*n - 1  if n>1 and n%4 <= 1   for n>=4
   (The subtraction occurs for n ≡ 0 or 1 (mod 4), n>1)
   This closed‑form matches the recurrence from the OEIS entry.  */
seq_t a(idx_t n)
{
    /* Special case a(0) */
    if (n == 0)
        return 1;

    /* Compute 2*n^2 + 2*n in seq_t */
    seq_t nseq = (seq_t)n;
    seq_t term = nseq * nseq * 2 + nseq * 2;

    /* Subtract 1 for n ≡ 0 or 1 (mod 4), with n>1 */
    if (n > 1 && (n % 4) <= 1)
        term = term - (seq_t)1;

    return term;
}
