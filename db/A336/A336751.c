/*
 * A336751  Smallest side of integer-sided triangles whose sides a < b < c are
 *          in arithmetic progression.
 * https://oeis.org/A336751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A336750(n,1) (smallest side of integer‑sided triangle
 *            with sides a<b<c in arithmetic progression, listed by increasing
 *            perimeter 3b, breaking ties by a)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A336751. */
seq_t a(idx_t n) {
    idx_t target = n;
    idx_t cumulative = 0;
    for (idx_t b = 1;; ++b) {
        idx_t count_b;
        if (b <= 2)
            count_b = 0;
        else if (b & 1)            /* b odd */
            count_b = (b - 1) / 2;
        else                       /* b even */
            count_b = (b - 2) / 2;

        if (cumulative + count_b >= target) {
            /* term lies in this b */
            idx_t offset = target - cumulative;               /* 1‑based */
            seq_t a_start = (seq_t)(b / 2) + 1;               /* floor(b/2)+1 */
            return a_start + (seq_t)(offset - 1);
        }
        cumulative += count_b;
    }
}
