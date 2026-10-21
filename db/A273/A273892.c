/*
 * A273892  Numbers starting with an even (decimal) digit.
 * https://oeis.org/A273892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th number whose first decimal digit is even (0,2,4,6,8). */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety against non‑positive calls */

    /* zero‑based index of the desired element */
    idx_t m = n - 1;

    /* numbers with a single digit: 0,2,4,6,8 */
    static const seq_t digits_len1[5] = {0, 2, 4, 6, 8};

    if (m < 5) return digits_len1[m];

    /* Find the decimal length k of the requested number.
       cumulative counts processed so far, pow10 = 10^(k-1) */
    idx_t cumulative = 5;   /* counts up to length 1 */
    idx_t pow10      = 1;   /* 10^(k-1), starts with 10^1 = 10 for k=2 */

    idx_t k = 1;
    while (1) {
        ++k;
        pow10 *= 10;                         /* 10^(k-1) for this length */
        idx_t count_k = 4 * pow10;           /* 4 * 10^(k-1) numbers of this length */

        if (m < cumulative + count_k) {
            idx_t offset  = m - cumulative;   /* position within this block   */
            idx_t block   = offset / pow10;   /* which leading even digit     */
            idx_t suffix  = offset % pow10;   /* remaining suffix digits      */
            seq_t lead    = (seq_t)(2 + 2 * block);   /* 2,4,6,8 */
            return (seq_t)(lead * pow10 + suffix);
        }
        cumulative += count_k;   /* move to next block */
    }
}
