/*
 * A266795  Total number of OFF (white) cells after n iterations of the "Rule
 *          61" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=4, a(3)=6. For n>3: if n even: a(n) =
 *            (n*(n+2)-2)/2 if n odd : a(n) = (n*n+5)/2
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    switch (n) {
        case 0: return (seq_t)0;
        case 1: return (seq_t)1;
        case 2: return (seq_t)4;
        case 3: return (seq_t)6;
        default: {
            seq_t N = (seq_t)n;
            if ((n & 1) == 0) {
                /* n even: (n*(n+2)-2)/2 */
                return (N * (N + (seq_t)2) - (seq_t)2) / (seq_t)2;
            } else {
                /* n odd: (n*n+5)/2 */
                return (N * N + (seq_t)5) / (seq_t)2;
            }
        }
    }
}
