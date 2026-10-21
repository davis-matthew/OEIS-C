/*
 * A283208  Minimal exponent integer sequence associated with Vietoris
 *          sequence.
 * https://oeis.org/A283208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Σ_{j=1..⌊log₂(n+1)⌋} ⌊(n+1)/2^j⌋
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The summation can be accumulated by repeatedly halving (n+1). */
seq_t a(idx_t n)
{
    seq_t sum = n;                      /* start with the n term          */
    idx_t m = n + 1;                    /* m = n+1                         */
    for (idx_t j = 1; (m >> j) != 0; ++j)
        sum += (seq_t)(m >> j);         /* add floor((n+1)/2^j)            */
    return sum;
}
