/*
 * A323220  a(n) = n*(n + 5)*(n + 7)*(n + 10)/24 + 1.
 * https://oeis.org/A323220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 5)*(n + 7)*(n + 10)/24 + 1
 * Range:     u8 n <= 0, u16 n <= 10, u32 n <= 383, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the product in steps to avoid overflow on intermediate values
       for typical 64‑bit seq_t sizes.  The intermediate results stay below
       2^64 for all n up to 10,000, which is the practical limit required. */
    seq_t t = (seq_t)n * ((seq_t)n + (seq_t)5);
    t *= ((seq_t)n + (seq_t)7);
    t *= ((seq_t)n + (seq_t)10);
    t /= (seq_t)24;
    t += (seq_t)1;
    return t;
}
