/*
 * A294020  Total number of elements after n-th stage of a hybrid cellular
 *          automaton formed by D-toothpicks and toothpicks (see Comments
 *          lines for precise definition).
 * https://oeis.org/A294020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-6) - a(n-7) (n > 10) Initial values: a(0)=0,
 *            a(1)=1, a(2)=5, a(3)=9, a(4)=15, a(5)=23, a(6)=27, a(7)=41,
 *            a(8)=65, a(9)=81, a(10)=103 The incremental differences after
 *            n≥7 are periodic with period 6: 14, 24, 16, 22, 8, 4. Hence for
 *            n≥6: a(n) = 27 + 88*((n-6)/6) + prefix[(n-6)%6...
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small‑term lookup */
    static const seq_t init[11] = {
        0ULL, 1ULL, 5ULL, 9ULL, 15ULL, 23ULL, 27ULL,
        41ULL, 65ULL, 81ULL, 103ULL
    };

    if (n <= 10) {
        return init[n];
    }

    /* n > 10, use the closed formula derived from the periodic
       difference sequence. */
    idx_t offset = n - 6;                       /* distance from a(6) */
    idx_t cycles = offset / 6;                  /* full 6‑step blocks */
    idx_t rem    = offset % 6;                  /* partial block */

    /* partial sums of first r elements of the period {14,24,16,22,8,4} */
    static const seq_t pref[7] = {
        0ULL, 14ULL, 38ULL, 54ULL, 76ULL, 84ULL, 88ULL
    };

    seq_t partial = pref[rem];                     /* sum of remaining diffs */
    seq_t term    = (seq_t)27 + ((seq_t)88 * (seq_t)cycles) + partial;
    return term;
}
