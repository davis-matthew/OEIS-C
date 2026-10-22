/*
 * A329684  Number of excursions of length n with Motzkin-steps forbidding all
 *          consecutive steps of length 2 except UD and HH.
 * https://oeis.org/A329684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = (n == 2) ? 2 : 1
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is 1, 1, 2, 1, 1, 1, ... for all n ≥ 0. */
    if (n == 2) {
        return (seq_t)2;
    } else {
        return (seq_t)1;
    }
}
