/*
 * A386431  Triangle T(n,k) read by rows, where row n contains the numbers 1,
 *          ..., n^2, written columnwise into a square matrix and read
 *          rowwise.
 * https://oeis.org/A386431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = T(r,k) where r is the smallest integer with 1²+2²+…+r² ≥
 *            n, k = n – (1²+2²+…+(r–1)²); T(r,k) = 1 + r·k – r –
 *            (r²–1)·⌊(k–1)/r⌋
 * Range:     u8 n <= 1495, u16 n <= 9862, u32 n <= 9862, u64 n <= 9862,
 *            u128 n <= 9862, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against an invalid index (offset 1). */
    if (n == 0) return 0;

    /* Find the row r such that sum_{i=1}^{r-1} i² < n ≤ sum_{i=1}^{r} i². */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum < n) {
        r++;
        sum += r * r;          /* add r² to the running total */
    }

    /* k is the position within row r (1‑based). */
    idx_t sum_prev = sum - r * r;  /* sum_{i=1}^{r-1} i² */
    idx_t k = n - sum_prev;

    /* Compute T(r,k) using the integer formula. */
    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;
    seq_t m = (kk - (seq_t)1) / rr;          /* floor((k-1)/r) */
    seq_t res = (seq_t)1 + rr * kk - rr
                - (rr * rr - (seq_t)1) * m;  /* 1 + r*k – r – (r²–1)*m */

    return res;
}
