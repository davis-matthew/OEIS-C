/*
 * A313561  Coordination sequence Gal.4.53.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 4*cnt4(n) + 5*(n - cnt4(n)), where cnt4(n) is the
 *            number of indices i ≤ n with i = 1, 3, 6, 11, 14, 19, …
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Count how many of the “4‑step” indices are ≤ n */
    idx_t cnt = 0;
    idx_t k   = 1;  /* index in the sequence of 4‑step positions */
    while (1) {
        idx_t t;
        if (k == 1)
            t = 1;
        else if (k == 2)
            t = 3;
        else
            t = 5 * k - 7 - 2 * ((k - 1) / 2);

        if (t > n)
            break;

        ++cnt;
        ++k;
    }

    /* a(n) = 1 + 4*cnt + 5*(n - cnt) */
    seq_t result = (seq_t)1
                 + (seq_t)4 * cnt
                 + (seq_t)5 * ((seq_t)n - cnt);

    return result;
}
