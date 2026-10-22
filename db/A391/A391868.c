/*
 * A391868  Start with the list of positive integers 1,2,3,... . After each
 *          integer k, insert k empty positions. Fill these positions, from
 *          left to right, with the positive integers in order. Repeat this
 *          spacing-and-filling process indefinitely. The sequence consists of
 *          the positions of the last occurrence o...
 * https://oeis.org/A391868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(1) = 1,  a(2) = 2
 *   For n ≥ 3:
 *     a(n) = a(n-1) + S(n-1) - 1
 *     S(n) = S(n-1) + T_{S(n-1)-1}
 *   where T_k = k(k+1)/2 is the k‑th triangular number.
 * This recurrence follows the definition of A391868.
 */
seq_t a(idx_t n) {
    /* Handle the offset explicitly. */
    if (n == 0) return 0;            /* undefined, but safe */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t a_val = (seq_t)2;  /* a(2) */
    seq_t S = (seq_t)2;      /* S(2) */

    for (idx_t i = 3; i <= n; ++i) {
        /* a(n) = a(n-1) + S(n-1) - 1 */
        seq_t new_a = a_val + S - 1;

        /* T_{S-1} = (S-1)*S/2 */
        seq_t k = S - 1;
        seq_t tri = k * S / 2;

        /* S(n) = S(n-1) + T_{S-1} */
        seq_t new_S = S + tri;

        a_val = new_a;
        S = new_S;
    }
    return a_val;
}
