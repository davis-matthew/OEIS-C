/*
 * A261151  a(n) = 11410337850553 + (n-1)*4609098694200.
 * https://oeis.org/A261151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11410337850553 + (n-1)*460909869420
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Constants for the arithmetic progression */
    static const seq_t BASE = (seq_t)11410337850553ULL;
    static const seq_t STEP = (seq_t)460909869420ULL;

    seq_t idx = (seq_t)n;            /* Convert index to seq_t */
    return BASE + (idx - (seq_t)1) * STEP;
}
