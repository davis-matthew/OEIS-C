/*
 * A394092  Negated least part of the partial alternating sums of the n-th
 *          composition in standard order (row n of A391983).
 * https://oeis.org/A394092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = negated minimal partial alternating sum of the
 *            composition derived from n by the standard order encoding
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* collect 1‑bit positions (1‑based) of n */
    int pos[65];
    int poscnt = 0;
    pos[0] = 0;
    idx_t t = n;
    int bitpos = 1;
    while (t != 0) {
        if (t & ((idx_t)1)) {
            pos[++poscnt] = bitpos;
        }
        t >>= 1;
        bitpos++;
    }
    if (poscnt == 0) return 0;

    /* compute composition parts (differences between successive positions) */
    seq_t parts[65];
    for (int i = 1; i <= poscnt; ++i)
        parts[i-1] = (seq_t)(pos[i] - pos[i-1]);

    /* compute minimal partial alternating sum */
    long long cur = 0;       /* current alternating sum S_j   */
    long long min_part = 0;  /* minimal value seen (≤ 0)      */

    for (int i = poscnt-1; i >= 0; --i) {
        long long q = (long long)parts[i];
        cur = -cur + q;            /* S_j = -S_{j-1} + q_j */
        if (cur < min_part) min_part = cur;
    }

    return (seq_t)(-min_part);    /* negate the least partial sum */
}
