/*
 * A269209  Number of n X 3 0..3 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling three exactly once.
 * https://oeis.org/A269209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1) - 237*a(n-2) + 180*a(n-3) - 36*a(n-4) for n>5
 *            Initial values: a(1)=24, a(2)=768, a(3)=18048, a(4)=361728,
 *            a(5)=6712704
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is undefined for n < 1; return 0 in that case. */
    if (n == 0) return (seq_t)0;

    /* Base values up to n = 5. */
    static const seq_t init[6] = {
        (seq_t)0,       /* init[0] unused, keeps indexing 1‑based */
        (seq_t)24,
        (seq_t)768,
        (seq_t)18048,
        (seq_t)361728,
        (seq_t)6712704
    };

    if (n <= 5) return init[n];

    /* Recurrence variables: t1 = a(k-1), t2 = a(k-2), t3 = a(k-3), t4 = a(k-4). */
    seq_t t1 = init[5];  /* a(5) */
    seq_t t2 = init[4];  /* a(4) */
    seq_t t3 = init[3];  /* a(3) */
    seq_t t4 = init[2];  /* a(2) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t an = (seq_t)30 * t1 - (seq_t)237 * t2 + (seq_t)180 * t3 - (seq_t)36 * t4;
        /* Shift for next step: a(k-2) becomes a(k-3), etc. */
        t4 = t3;
        t3 = t2;
        t2 = t1;
        t1 = an;
    }
    return t1;  /* t1 now holds a(n) */
}
