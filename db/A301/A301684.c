/*
 * A301684  Coordination sequence for node of type V2 in "krg" 2-D tiling (or
 *          net).
 * https://oeis.org/A301684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=8, a(3)=12, a(4)=18, a(5)=22, a(6)=28,
 *            a(7)=34, a(8)=36. For n ≥ 9: a(n) = a(n‑1) + a(n‑3) - a(n‑4)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first nine terms – used as base cases. */
    static const seq_t init[9] = {1, 4, 8, 12, 18, 22, 28, 34, 36};

    if (n < 9)
        return init[n];

    /*
     * Sliding window containing a_{k-4}, a_{k-3}, a_{k-2}, a_{k-1}
     * where k is the current n being computed.
     * For k = 9 (first iteration) the window is [a5, a6, a7, a8].
     */
    seq_t w0 = init[5];   /* a_{k-4} */
    seq_t w1 = init[6];   /* a_{k-3} */
    seq_t w2 = init[7];   /* a_{k-2} */
    seq_t w3 = init[8];   /* a_{k-1} */

    seq_t cur;
    for (idx_t k = 9; k <= n; ++k) {
        /* a(k) = a(k-1) + a(k-3) - a(k-4) */
        cur = w3 + w1 - w0;
        /* slide the window for next iteration */
        w0 = w1;
        w1 = w2;
        w2 = w3;
        w3 = cur;
    }
    return w3;   /* a(n) after loop */
}
