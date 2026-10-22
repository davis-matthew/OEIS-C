/*
 * A305185  a(n) minimizes the maximum norm of elements in a complete residue
 *          system of Eisenstein integers modulo n.
 * https://oeis.org/A305185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0; for n ≥ 2, a(n) = n² − 3 n ceil(n/3) + 3 ceil(n/3)²,
 *            where ceil(n/3) = (n + 2)/3
 * Range:     u8 n <= 28, u16 n <= 443, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1)
        return 0;                           // a(1) = 0
    seq_t N = (seq_t)n;
    seq_t k = (N + 2) / 3;                  // ceil(n/3)
    return N * N - 3 * N * k + 3 * k * k;   // formula
}
