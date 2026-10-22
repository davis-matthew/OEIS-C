/*
 * A304584  A linear mapping a(n) = x + d*n of pairs of nonnegative integers
 *          (x,d), where the pairs are enumerated by antidiagonals.
 * https://oeis.org/A304584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   enumerate pairs (x,d) of non‑negative integers by antidiagonals
 *            in * descending x order, and define a(n) = x + d * n. For each n
 *            ≥ 0, * let k be the unique integer such that k*(k+1)/2 ≤ n <
 *            (k+1)*(k+2)/2. * Within the k‑th diagonal let j = n - k*(k+1)/2,
 *            so the * n‑th pair is (x, d) = (k-j, j) an...
 * Range:     u8 n <= 42, u16 n <= 1324, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find k: the diagonal containing index n */
    idx_t low = 0, high = 1;
    while ((high * (high + 1)) / 2 <= n) {
        high <<= 1;                 /* double until we pass n */
    }
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;   /* upper mid */
        if ((mid * (mid + 1)) / 2 <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    idx_t k = low;

    /* Position within diagonal */
    idx_t t = (k * (k + 1)) / 2;    /* number of pairs before diagonal k */
    idx_t j = n - t;                /* 0‑based index inside diagonal k */

    /* Coordinates of the n‑th pair */
    idx_t x = k - j;                /* x = k - j */
    idx_t d = j;                    /* d = j */

    /* Compute a(n) = x + d * n (all ops in seq_t) */
    seq_t result = (seq_t)x + (seq_t)d * (seq_t)n;
    return result;
}
