/*
 * A329683  Number of excursions of length n with Motzkin-steps forbidding all
 *          consecutive steps of length 2 except UH, HH and HD.
 * https://oeis.org/A329683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 1 for n < 3, 2 otherwise
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)1;
    return (seq_t)2;
}
