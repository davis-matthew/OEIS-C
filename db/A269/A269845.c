/*
 * A269845  Irregular triangle read by rows: T(n,k) = (k/2+1/2)^2 if odd-k
 *          otherwise T(n,k) = (n-k/2)^2 where n >= 1, k = 0..2*n-1.
 * https://oeis.org/A269845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) defined by the irregular triangle where for n≥1, k=0..2n-1,
 *            T(n,k) = (n - k/2)^2 if k even ((k+1)/2)^2 correct for k odd The
 *            sequence a(n) lists the triangle entries row‑by‑row
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t idx)
{
    /* Find the row n such that n(n+1) ≥ idx > (n-1)n. */
    idx_t low = 1;
    idx_t high = idx;              /* high is sufficient: n ≤ idx */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        if (prod >= idx)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t n = low;                 /* the desired row */

    /* previous total entries before this row */
    unsigned __int128 prev_total_128 = (unsigned __int128)(n - 1) * n;
    idx_t prev_total = (idx_t)prev_total_128;

    /* position of k within the row (0‑based) */
    idx_t k = idx - prev_total - 1;

    seq_t v;  /* intermediate value before squaring */
    if ((k & 1) == 0) {            /* even k: (n - k/2)^2 */
        v = (seq_t)n - ((seq_t)(k >> 1));
    } else {                       /* odd k: ((k+1)/2)^2 */
        v = ((seq_t)(k >> 1) + 1);
    }
    return v * v;
}
