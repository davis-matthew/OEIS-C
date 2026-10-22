/*
 * A329670  Number of excursions of length n with Motzkin-steps allowing only
 *          consecutive steps UH and HD.
 * https://oeis.org/A329670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = coefficient of t^n in the formal power series
   G(t) = 1 + t + t^3.
   Hence a(0)=1, a(1)=1, a(3)=1, and a(n)=0 for all other n. */
seq_t a(idx_t n)
{
    /* Use a switch on the unsigned integer value of n */
    switch ((uint64_t)n) {
    case 0:
        return (seq_t)1;
    case 1:
        return (seq_t)1;
    case 3:
        return (seq_t)1;
    default:
        return (seq_t)0;
    }
}
