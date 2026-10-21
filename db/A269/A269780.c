/*
 * A269780  Table read by antidiagonals where T(i,j) is the location of the
 *          pair (i,j) in A269501.
 * https://oeis.org/A269780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j), where the pair (i,j) is the n‑th entry of the
 *            table read by antidiagonals; T(i,j) is given by: - if i==1:
 *            (j-1)^2 + 1 - else if i>j: i^2 - 2*j + 2 - otherwise: j^2 - 2*i
 *            + 3
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n==0, though the sequence is 1‑based. */
    if (n == 0) return 0;

    /* Find the smallest k such that k*(k-1)/2 >= n.
       The diagonal with i+j = k contains k-1 entries. */
    idx_t k_low = 1;
    idx_t k_high = 2;
    /* Increase k_high until the cumulative count reaches n. */
    while ( ((unsigned __int128)k_high * (k_high - 1)) / 2 < n )
        k_high <<= 1;  /* double k_high */

    /* Binary search between k_low and k_high. */
    while (k_low < k_high) {
        idx_t mid = k_low + ((k_high - k_low) >> 1);
        unsigned __int128 mid_val = ((unsigned __int128)mid * (mid - 1)) / 2;
        if (mid_val >= n) k_high = mid;
        else               k_low  = mid + 1;
    }
    idx_t k = k_low;  /* i + j = k */

    /* Total entries before this diagonal. */
    unsigned __int128 prev_val = ((unsigned __int128)(k - 1) * (k - 2)) / 2;
    idx_t prev = (idx_t)prev_val;        /* prev < n */
    idx_t offset = n - prev;             /* 1 … k-1 */

    idx_t i = offset;                     /* 1‑based */
    idx_t j = k - offset;                 /* 1‑based */

    /* Compute T(i,j) according to the piecewise definition. */
    if (i == 1) {
        /* (j-1)^2 + 1 */
        seq_t t = (seq_t)(j - 1);
        return t * t + (seq_t)1;
    } else if (i > j) {
        /* i^2 - 2*j + 2 */
        seq_t ti = (seq_t)i;
        return ti * ti - (seq_t)2 * (seq_t)j + (seq_t)2;
    } else {
        /* j^2 - 2*i + 3 */
        seq_t tj = (seq_t)j;
        return tj * tj - (seq_t)2 * (seq_t)i + (seq_t)3;
    }
}
