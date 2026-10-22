/*
 * A373742  Expansion of e.g.f. exp(x^3/6 * (1 + x)).
 * https://oeis.org/A373742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A373742 - Expansion of e.g.f. exp(x^3/6 · (1+x))
   a(0) = 1, a(1) = a(2) = 0, a(3) = 1
   a(n) = ((n-1)*(n-2)/6) * (3*a(n-3) + 4*(n-3)*a(n-4))   for n ≥ 4 */
seq_t a(idx_t n)
{
    /* small ring to store the last four values a(i-4)…a(i-1) */
    seq_t ring[4] = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 };

    if (n <= 3)
        return ring[n];

    for (idx_t i = 4; i <= n; ++i) {
        seq_t a_n3 = ring[(i - 3) & 3];   /* a(i-3) */
        seq_t a_n4 = ring[(i - 4) & 3];   /* a(i-4) */

        /* use 128‑bit intermediate to avoid overflow on 64‑bit builds */
        __int128 num   = (__int128)(i - 1) * (i - 2);
        __int128 inner = (__int128)3 * (__int128)a_n3
                       + (__int128)4 * (__int128)(i - 3) * (__int128)a_n4;
        __int128 val   = num * inner / 6;

        ring[i & 3] = (seq_t)val;
    }

    return ring[n & 3];
}
