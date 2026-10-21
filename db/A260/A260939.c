/*
 * A260939  Thirteen primes in arithmetic progression with difference 60060
 *          and minimal initial term.
 * https://oeis.org/A260939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 4943 + (n-1)*60060
 * Range:     u8 none, u16 n <= 2, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the nth term of the arithmetic progression */
    seq_t base  = (seq_t)4943;
    seq_t diff  = (seq_t)60060;
    seq_t idxm1 = (seq_t)n - 1;
    return base + idxm1 * diff;
}
