/*
 * A312656  Coordination sequence Gal.6.201.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n ≥ 1, let c(n) be the number of indices i < n for
 *            which the difference a(i+1) – a(i) equals 6. Then a(n) = 4·n +
 *            2·c(n). The indices i for which the difference is 6 occur in
 *            pairs starting at i = 2,7,13,18,24,29,…, where successive
 *            starting positions alternate by 5 and 6 (i.e. the...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    /* count indices i < n where a(i+1)-a(i) = 6 */
    idx_t count = 0;
    idx_t start = 2;          /* first start of a diff‑6 pair */
    int toggle = 0;           /* 0 → add 5, 1 → add 6 */

    while (start < n) {
        if (start + 1 < n)
            count += 2;          /* both indices of the pair are < n */
        else
            count += 1;          /* only the first index is < n */

        start += (toggle == 0 ? 5 : 6);
        toggle ^= 1;            /* alternate 5,6,5,6,… */
    }

    /* a(n) = 4·n + 2·count */
    return (seq_t)4 * (seq_t)n + (seq_t)2 * (seq_t)count;
}
