/*
 * A266440  Total number of OFF (white) cells after n iterations of the "Rule
 *          23" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + ((-1)^n + 2)*n + (-1)^n - 1)/2
 * Range:     u8 n <= 15, u16 n <= 279, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t num;
    if ((n & (idx_t)1) == 0) {          // n even
        num = N * N + 3 * N;
    } else {                            // n odd
        num = N * N + N - 2;
    }
    return num / 2;
}
