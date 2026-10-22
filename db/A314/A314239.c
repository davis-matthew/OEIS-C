/*
 * A314239  Coordination sequence Gal.6.642.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = a(n-1) + s(n) for n>0 s(1) = 4, s(2) = 6 for n ≥
 *            3: s(n) = s(n-1) + c[(n-3) mod 11] where c = {0, 0, -1, 1, -1,
 *            1, 0, 0, -1, 0, 1}
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A314239 (Coordination sequence Gal.6.642.2) */
seq_t a(idx_t n)
{
    if (n == 0)                      /* first term */
        return 1;

    /* periodic correction of the first differences of s(n) */
    static const int8_t cycle[11] = {
        0, 0, -1, 1, -1, 1, 0, 0, -1, 0, 1
    };

    seq_t a_val = 1;                 /* a(0)            */
    seq_t step  = 4;                 /* s(1)            */
    a_val += step;                   /* a(1)            */

    if (n == 1)                      /* already computed */
        return a_val;

    for (idx_t i = 2; i <= n; ++i) {
        int add;                     /* correction to step */
        if (i == 2)
            add = 2;                 /* s(2)-s(1) = 6-4 = 2 */
        else
            add = cycle[(i - 3) % 11];

        step += (seq_t)add;          /* s(i) = s(i-1) + add */
        a_val += step;               /* a(i) = a(i-1) + s(i) */
    }

    return a_val;
}
