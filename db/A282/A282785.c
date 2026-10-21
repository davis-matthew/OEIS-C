/*
 * A282785  Number of n X 2 0..1 arrays with no 1 equal to more than one of
 *          its king-move neighbors, with the exception of exactly one
 *          element.
 * https://oeis.org/A282785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+5*a(n-2)+2*a(n-3)-17*a(n-4)-24*a(n-5)-16*a(n-6)
 *            Initial terms: a(1)=0, a(2)=0, a(3)=8, a(4)=16, a(5)=72,
 *            a(6)=240
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive indices (not part of the sequence). */
    if (n == 0) return 0;

    /* Known first six values. */
    static const seq_t init[6] =
    {
        (seq_t)0,   /* a(1) */
        (seq_t)0,   /* a(2) */
        (seq_t)8,   /* a(3) */
        (seq_t)16,  /* a(4) */
        (seq_t)72,  /* a(5) */
        (seq_t)240  /* a(6) */
    };

    if (n <= 6) return init[n-1];

    /* Working array holding the last 6 values. */
    seq_t v[6];
    v[0] = init[0];
    v[1] = init[1];
    v[2] = init[2];
    v[3] = init[3];
    v[4] = init[4];
    v[5] = init[5];

    for (idx_t i = 7; i <= n; ++i)
    {
        seq_t pos = v[5] * 2ULL;   /* 2*a(n-1) */
        pos += v[4] * 5ULL;        /* +5*a(n-2) */
        pos += v[3] * 2ULL;        /* +2*a(n-3) */

        seq_t sub = v[2] * 17ULL;  /* 17*a(n-4) */
        sub += v[1] * 24ULL;       /* +24*a(n-5) */
        sub += v[0] * 16ULL;       /* +16*a(n-6) */

        seq_t next = pos - sub;    /* a(n) */

        /* Shift the window of previous values. */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }

    return v[5];
}
