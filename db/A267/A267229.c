/*
 * A267229  Number of length-n 0..5 arrays with no following elements greater
 *          than or equal to the first repeated value.
 * https://oeis.org/A267229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the 6‑term linear recurrence a(1)=6, a(2)=36,
 *            a(3)=195, a(4)=1030, a(5)=5375, a(6)=27854, a(7)=143695 a(n) =
 *            20*a(n-1) - 160*a(n-2) + 650*a(n-3) - 1399*a(n-4) + 1490*a(n-5)
 *            - 600*a(n-6) for all n > 7. This recurrence exactly generates
 *            the sequence A267229
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values a(1)..a(7) */
    static const seq_t init[7] = {6, 36, 195, 1030, 5375, 27854, 143695};

    if (n <= 7)
        return init[n - 1];

    /* Use a sliding window of the last six terms:
       prev[0] = a(n-1), prev[1] = a(n-2), ..., prev[5] = a(n-6) */
    seq_t prev[6];
    prev[0] = init[6]; /* a7   */
    prev[1] = init[5]; /* a6   */
    prev[2] = init[4]; /* a5   */
    prev[3] = init[3]; /* a4   */
    prev[4] = init[2]; /* a3   */
    prev[5] = init[1]; /* a2   */

    seq_t cur = 0;
    for (idx_t i = 8; i <= n; ++i) {
        cur = 20 * prev[0]
            - 160 * prev[1]
            + 650 * prev[2]
            - 1399 * prev[3]
            + 1490 * prev[4]
            - 600  * prev[5];

        /* Shift the window to prepare for the next iteration */
        prev[5] = prev[4];
        prev[4] = prev[3];
        prev[3] = prev[2];
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = cur;
    }
    return cur;
}
