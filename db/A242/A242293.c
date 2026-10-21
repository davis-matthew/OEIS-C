/*
 * A242293  Greedy residue sequence of cubes 2^3, 3^3, 4^3, ...
 * https://oeis.org/A242293
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 – (sum of all distinct cubes m^3 (1 ≤ m < n) that can
 *            be subtracted greedily from n^3). Equivalently, starting with v
 *            = n^3 and for m = n−1 down to 1 if m^3 ≤ v then v -= m^3; after
 *            the loop v is a(n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1698, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* Offset starts at 2 */

    /* Compute n^3 in seq_t */
    seq_t n_seq = (seq_t)n;
    seq_t v = n_seq * n_seq * n_seq;

    /* Greedy subtraction of cubes from n-1 down to 1 */
    for (idx_t m = n - 1; m >= 1; --m) {
        seq_t cube = (seq_t)m * (seq_t)m * (seq_t)m;
        if (cube <= v) v -= cube;
    }
    return v;
}
