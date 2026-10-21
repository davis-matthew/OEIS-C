/*
 * A288477  a(n) = (2^49 - 2)*n/3 + 247371098957.
 * https://oeis.org/A288477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^49-2)*n/3 + 247371098957
 * Range:     u8 none, u16 none, u32 none, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 247371098957, a(1) = 187897355572727, ... */

seq_t a(idx_t n)
{
    /* (2^49-2)/3 = 187649984473770 */
    static const seq_t base   = (seq_t)187649984473770ULL;
    static const seq_t offset = (seq_t)247371098957ULL;
    return offset + base * (seq_t)n;
}
