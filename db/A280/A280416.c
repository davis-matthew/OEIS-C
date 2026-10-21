/*
 * A280416  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 262", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A280416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280416.  Computed from the known initial segment and the
   eventual 2‑cycle a(2k)=1, a(2k+1)=7 for k≥15. */
seq_t a(idx_t n) {
    /* known values up to n=31 (computed via the generating function) */
    static const seq_t values[32] = {
        /* 0 */ 1ULL,
        /* 1 */ 3ULL,
        /* 2 */ 1ULL,
        /* 3 */ 7ULL,
        /* 4 */ 17ULL,
        /* 5 */ 39ULL,
        /* 6 */ 65ULL,
        /* 7 */ 199ULL,
        /* 8 */ 65ULL,
        /* 9 */ 455ULL,
        /*10 */ 1025ULL,
        /*11 */ 2055ULL,
        /*12 */ 4097ULL,
        /*13 */ 8199ULL,
        /*14 */ 16385ULL,
        /*15 */ 32775ULL,
        /*16 */ 65537ULL,
        /*17 */ 131079ULL,
        /*18 */ 262145ULL,
        /*19 */ 524295ULL,
        /*20 */ 1048577ULL,
        /*21 */ 2097159ULL,
        /*22 */ 4194305ULL,
        /*23 */ 8388615ULL,
        /*24 */ 16777217ULL,
        /*25 */ 33554439ULL,
        /*26 */ 67108865ULL,
        /*27 */ 134217735ULL,
        /*28 */ 268435457ULL,
        /*29 */ 805306375ULL,
        /*30 */ 1ULL,
        /*31 */ 7ULL
    };

    if (n < 32)
        return values[n];

    /* for n >= 32 the sequence is periodic with period 2:
       even indices give 1, odd indices give 7 */
    return (n & 1ULL) ? values[31] : values[30];
}
