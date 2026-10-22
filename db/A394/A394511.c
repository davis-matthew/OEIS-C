/*
 * A394511  Minimum number of cells in a connected enclosing shell of an
 *          n-cell connected hole on the truncated-square (4.8.8) Archimedean
 *          tiling.
 * https://oeis.org/A394511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*ceil((n + 17)/5) for n >= 6 Initial values: a(1)=4,
 *            a(2)=8, a(3)=8, a(4)=8, a(5)=8. (The recurrence a(n)=a(n-5)+2
 *            for n>=11 follows from this.)
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 6) {
        static const seq_t table[6] = {0, 4, 8, 8, 8, 8};
        return table[n];
    }
    /* For n >= 6 use a(n) = 2 * floor((n + 21)/5) == 2*ceil((n + 17)/5) */
    idx_t t    = n + 21;          /* (n + 17) + (5 - 1) */
    idx_t ceil = t / 5;           /* floor((n + 21)/5) == ceil((n + 17)/5) */
    return (seq_t)(ceil * 2);
}
