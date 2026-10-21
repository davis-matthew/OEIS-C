/*
 * A269328  An eventually quasilinear solution to Hofstadter's Q recurrence.
 * https://oeis.org/A269328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(4)=3; a(5n) = 6n, a(5n+1)= 5, a(5n+2)= 2, a(5n+3)= 5n,
 *            a(5n+4)= 5 (for n ≥ 1)
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 4)
        return (seq_t)3;
    idx_t k = n / 5;          /* integer division, k = floor(n/5) */
    switch (n % 5) {
        case 0:              /* n = 5k, n ≥ 5 */
            return (seq_t)6 * (seq_t)k;
        case 1:              /* n = 5k+1 */
            return (seq_t)5;
        case 2:              /* n = 5k+2 */
            return (seq_t)2;
        case 3:              /* n = 5k+3 */
            return (seq_t)5 * (seq_t)k;
        case 4:              /* n = 5k+4, n≠4 handled above */
            return (seq_t)5;
    }
    /* the switch should cover all residues, but keep compiler quiet */
    return (seq_t)0;
}
