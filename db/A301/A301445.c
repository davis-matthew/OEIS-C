/*
 * A301445  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          5 or 6 horizontally, vertically or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A301445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) +7*a(n-2) -10*a(n-3) -a(n-4) +7*a(n-5)
 *            -17*a(n-6) * +5*a(n-8) -11*a(n-9) +5*a(n-10) -2*a(n-12)
 *            +2*a(n-13); * Initial values: a(1)=4, a(2)=29, a(3)=171,
 *            a(4)=1008, a(5)=5930, * a(6)=34976, a(7)=206266, a(8)=1216562,
 *            a(9)=7175157, * a(10)=42318566, a(11)=249591629,
 *            a(12)=1472072549,...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the trivial cases directly. */
    if (n == 0) return 0; /* offset begins at 1, but still handle 0 gracefully */

    /* Base values for n ≤ 13. */
    static const seq_t init[13] = {
        4ULL,              /* a(1) */
        29ULL,             /* a(2) */
        171ULL,            /* a(3) */
        1008ULL,           /* a(4) */
        5930ULL,           /* a(5) */
        34976ULL,          /* a(6) */
        206266ULL,         /* a(7) */
        1216562ULL,        /* a(8) */
        7175157ULL,        /* a(9) */
        42318566ULL,       /* a(10) */
        249591629ULL,      /* a(11) */
        1472072549ULL,     /* a(12) */
        8682172154ULL      /* a(13) */
    };
    if (n <= 13) return init[n - 1];

    /* Circular buffer holding the last 13 computed values:
       buf[0] = a(k-12), ..., buf[12] = a(k) for current k.  */
    seq_t buf[13];
    for (idx_t i = 0; i < 13; ++i) buf[i] = init[i];

    /* Compute successively from a(14) up to a(n). */
    idx_t k = 14;
    for (; k <= n; ++k) {
        /* Map a(k-1) … a(k-13) to buffer indices. */
        const seq_t a1   = buf[12]; /* a(k-1) */
        const seq_t a2   = buf[11]; /* a(k-2) */
        const seq_t a3   = buf[10]; /* a(k-3) */
        const seq_t a4   = buf[9];  /* a(k-4) */
        const seq_t a5   = buf[8];  /* a(k-5) */
        const seq_t a6   = buf[7];  /* a(k-6) */
        const seq_t a8   = buf[5];  /* a(k-8) */
        const seq_t a9   = buf[4];  /* a(k-9) */
        const seq_t a10  = buf[3];  /* a(k-10) */
        const seq_t a12  = buf[1];  /* a(k-12) */
        const seq_t a13  = buf[0];  /* a(k-13) */

        /* Apply the recurrence explicitly to avoid intermediate negative
         * values in unsigned arithmetic. The intermed. sums stay within
         * the range of seq_t because the terms grow monotonically. */
        seq_t next = 5 * a1 + 7 * a2 - 10 * a3 - a4
                    + 7 * a5 - 17 * a6 + 5 * a8
                    - 11 * a9 + 5 * a10 - 2 * a12 + 2 * a13;

        /* Shift the buffer left and insert the new value. */
        for (idx_t i = 0; i < 12; ++i) buf[i] = buf[i + 1];
        buf[12] = next;
    }

    return buf[12];
}
