/*
 * A266723  Total number of ON (black) cells after n iterations of the "Rule
 *          59" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3 + (-1)^n - 2*(-3 + (-1)^n)*n + 2*n^2) / 4
 * Range:     u8 n <= 20, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    long long nn = (long long)n;                 /* n fits in 64‑bit            */
    long long s  = (nn & 1LL) ? -1LL : 1LL;      /* (-1)^n                      */
    long long tmp = -3 + s;                      /* -3 + (-1)^n                 */
    long long num = 3 + s - 2 * tmp * nn + 2 * nn * nn;
    long long res = num / 4;
    return (seq_t)res;
}
