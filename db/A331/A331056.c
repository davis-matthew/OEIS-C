/*
 * A331056  Coordination sequence for hexavalent vertex in 1-skeleton of
 *          disdyakis triacontahedron.
 * https://oeis.org/A331056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1 for n = 0 or 6, * 6 for n = 1 or 5, * 18 for n = 2 or
 *            4, * 12 for n = 3, * 0 otherwise (n > 6)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 6U) return (seq_t)0;
    switch ((unsigned)n) {
        case 0: case 6: return (seq_t)1;
        case 1: case 5: return (seq_t)6;
        case 2: case 4: return (seq_t)18;
        case 3:         return (seq_t)12;
        default:        return (seq_t)0; /* should not occur */
    }
}
