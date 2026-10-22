/*
 * A383722  a(n) = A378762(A382679(n)).
 * https://oeis.org/A383722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Define row index r by S(r-1) < n ≤ S(r) where S(r) =
 *            2r²–r. Let B = S(r-1) and let pos = n – B (1‑based in the row of
 *            length 4r–3). Let m = 2r–1. If pos < m: If pos is odd: P = 2m–1
 *            – pos; else P = pos. Else: If pos is odd: P = 3m–1 – pos; else P
 *            = 2m–1 – pos. Then a(n) = B + P
 * Range:     u8 n <= 231, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* safety for out of bounds, although not required */

    /* Find the row r containing position n. */
    idx_t r = 1;
    idx_t sum = 1;           /* S(1) = 1 */
    while (sum < n) {
        ++r;
        sum += 4 * r - 3;    /* add length of the new row */
    }

    /* Compute B = S(r-1) = 2*(r-1)² – (r-1). */
    idx_t r_minus1 = r - 1;
    idx_t sum_prev = 2 * r_minus1 * r_minus1 - r_minus1;

    /* Position within the row, 1‑based. */
    idx_t pos_idx = n - sum_prev;
    seq_t pos = (seq_t)pos_idx;

    /* m = 2r – 1 */
    seq_t m = 2 * ((seq_t)r) - 1;

    /* Compute the offset P according to the piecewise definition. */
    seq_t P;
    if (pos < m) {
        if (pos & 1U) {           /* pos is odd */
            P = 2 * m - 1 - pos;
        } else {                  /* pos is even */
            P = pos;
        }
    } else {
        if (pos & 1U) {           /* pos is odd */
            P = 3 * m - 1 - pos;
        } else {                  /* pos is even */
            P = 2 * m - 1 - pos;
        }
    }

    /* a(n) = B + P */
    return (seq_t)sum_prev + P;
}
