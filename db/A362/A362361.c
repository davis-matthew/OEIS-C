/*
 * A362361  a(n) = n*2^10 - 3.
 * https://oeis.org/A362361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = n*2^10 - 3
 * Range:     u8 none, u16 n <= 64, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type to preserve exactness
       for arbitrary‑precision builds. */
    seq_t k = (seq_t)n;
    /* 2^10 = 1024, a(n) = n*1024 - 3. */
    return k * (seq_t)1024 - (seq_t)3;
}
