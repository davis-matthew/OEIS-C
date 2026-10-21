/*
 * A242894  Beginning with a central 'Star' configuration of a Penrose 'Kite'
 *          and 'Dart' tiling with rotational symmetry as the first step,
 *          number of tiles that can be added to the free edges of the current
 *          tiling.
 * https://oeis.org/A242894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   Define r = (n-1) mod 5 and q = (n-1) / 5. a(n) = 5 + 20*q if r =
 *            0 10 + 25*q if r = 1 10 + 30*q if r = 2 20 + 20*q if r = 3 20 +
 *            25*q if r = 4 (All operations performed in seq_t.) This
 *            implements the recurrence a(n)=a(n-5)+d[n-5], where d =
 *            {20,25,30,20,25} with the base values a(1)=5, a(2)=10, a...
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = (n-1) / 5;            /* integer part */
    unsigned r = (unsigned)((n-1) % 5); /* remainder 0..4 */
    seq_t k = (seq_t)q;

    switch (r) {
        case 0: return (seq_t)5 + (seq_t)20 * k;
        case 1: return (seq_t)10 + (seq_t)25 * k;
        case 2: return (seq_t)10 + (seq_t)30 * k;
        case 3: return (seq_t)20 + (seq_t)20 * k;
        case 4: return (seq_t)20 + (seq_t)25 * k;
        default: return 0; /* unreachable */
    }
}
