/*
 * A334501  Eventual period of a single cell in rule 190 cellular automaton in
 *          a cyclic universe of width n.
 * https://oeis.org/A334501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-4) - a(n-8) for n >= 13. Initial values: a(1)=1,
 *            a(2)=1, a(3)=1, a(4)=4, a(5)=5, a(6)=6, a(7)=7, a(8)=4, a(9)=9,
 *            a(10)=10, a(11)=11, a(12)=4. For any n >= 1 this function
 *            returns a(n)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values indexed from 1 to 12.  Index 0 is unused. */
    static const seq_t base[13] = {
        0ULL,      /* 0 (unused) */
        1ULL,      /* a(1) */
        1ULL,      /* a(2) */
        1ULL,      /* a(3) */
        4ULL,      /* a(4) */
        5ULL,      /* a(5) */
        6ULL,      /* a(6) */
        7ULL,      /* a(7) */
        4ULL,      /* a(8) */
        9ULL,      /* a(9) */
        10ULL,     /* a(10) */
        11ULL,     /* a(11) */
        4ULL       /* a(12) */
    };

    /* For n <= 12, return the base value. */
    if (n <= 12) {
        return base[n];
    }

    /* Circular buffer of size 8 to store the last eight terms required
       by the recurrence a(n) = 2*a(n-4) - a(n-8). */
    seq_t buf[8];

    /* Initialise the buffer with the base values needed for indices
       1 through 12.  The index into the buffer is i % 8. */
    for (idx_t i = 1; i <= 8; ++i) {
        buf[i % 8] = base[i];
    }
    for (idx_t i = 9; i <= 12; ++i) {
        buf[i % 8] = base[i];
    }

    /* Compute sequentially from i = 13 up to the requested n. */
    for (idx_t i = 13; i <= n; ++i) {
        idx_t pos  = i % 8;
        idx_t pos4 = (i - 4) % 8;
        idx_t pos8 = (i - 8) % 8;
        buf[pos] = 2 * buf[pos4] - buf[pos8];
    }

    return buf[n % 8];
}
