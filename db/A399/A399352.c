/*
 * A399352  Number of independent vertex sets in the n X 2 projective plane
 *          graph.
 * https://oeis.org/A399352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 54, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-2) + a(n-4) - a(n-6)
 * Initial values: a(0)=1, a(1)=3, a(2)=5, a(3)=15, a(4)=27, a(5)=77 */
seq_t a(idx_t n) {
    const seq_t base[6] = {1, 3, 5, 15, 27, 77};
    if (n < 6) return base[n];

    seq_t arr[6];
    for (idx_t i = 0; i < 6; ++i) arr[i] = base[i];

    for (idx_t i = 6; i <= n; ++i) {
        idx_t i2 = (i + 4) % 6; // accesses a(i-2)
        idx_t i4 = (i + 2) % 6; // accesses a(i-4)
        idx_t i6 = i % 6;       // accesses a(i-6)
        seq_t next = 5 * arr[i2] + arr[i4] - arr[i6];
        arr[i % 6] = next;
    }

    return arr[n % 6];
}
