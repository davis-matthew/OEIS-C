/*
 * A356828  Number of vertex cuts in the n-ladder graph P_2 x P_n.
 * https://oeis.org/A356828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 8*a(n-1) - 20*a(n-2) + 16*a(n-3) + a(n-4) - 4*a(n-5)
 * Initial values: a(1)=0, a(2)=2, a(3)=23, a(4)=147, a(5)=748
 */
seq_t a(idx_t n)
{
    if (n <= 0)
        return (seq_t)0;          /* undefined for n < 1, return 0 */

    /* return small values directly */
    switch (n) {
        case 1: return (seq_t)0;
        case 2: return (seq_t)2;
        case 3: return (seq_t)23;
        case 4: return (seq_t)147;
        case 5: return (seq_t)748;
        default: break;
    }

    /* use a rolling window of the last five computed terms */
    seq_t t1 = (seq_t)0;   /* a(n-5) */
    seq_t t2 = (seq_t)2;   /* a(n-4) */
    seq_t t3 = (seq_t)23;  /* a(n-3) */
    seq_t t4 = (seq_t)147; /* a(n-2) */
    seq_t t5 = (seq_t)748; /* a(n-1) */

    seq_t val;
    for (idx_t i = 6; i <= n; ++i) {
        val = t5 * (seq_t)8            /* 8*a(i-1) */
            - t4 * (seq_t)20           /* -20*a(i-2) */
            + t3 * (seq_t)16           /* +16*a(i-3) */
            + t2                       /* +a(i-4) */
            - t1 * (seq_t)4;           /* -4*a(i-5) */

        /* shift window */
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = t5;
        t5 = val;
    }
    return t5;
}
