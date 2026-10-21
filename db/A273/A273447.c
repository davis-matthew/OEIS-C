/*
 * A273447  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 721", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 12*n^2 + 8*n - 39)/3 for n>2; a(0) = 1, a(1) =
 *            5, a(2) = 26
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)26;
    /* Use 128‑bit intermediate to avoid overflow on the various seq_t sizes */
    unsigned __int128 nn  = n;
    unsigned __int128 n2  = nn * nn;
    unsigned __int128 n3  = n2 * nn;
    unsigned __int128 num = 4 * n3 + 12 * n2 + 8 * nn - 39;
    unsigned __int128 res = num / 3;
    return (seq_t)res;
}
