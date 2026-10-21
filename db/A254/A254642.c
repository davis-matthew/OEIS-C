/*
 * A254642  Third partial sums of eighth powers (A001016).
 * https://oeis.org/A254642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(n+1)*(n+2)*(n+3)*(3+2*n)*(1+36*n-69*n^2+45*n^4+18*n^5+2*n^6)/3960
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 104, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses 128‑bit integer arithmetic for 64‑bit seq_t values to avoid
 * overflow, and otherwise uses seq_t operations directly.  All calculations are
 * performed using only integer operators, as required. */

#ifndef SEQ_T_BITS
#define SEQ_T_BITS (sizeof(seq_t) * 8)
#endif

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1 */

    /* ---------- 64‑bit seq_t (or smaller) --------------------------------- */
    if (SEQ_T_BITS <= 64) {
        __int128 N   = (__int128)n;          /* use 128‑bit intermediate   */
        __int128 N2  = N * N;
        __int128 N4  = N2 * N2;
        __int128 N5  = N4 * N;
        __int128 N6  = N5 * N;

        __int128 part1 = N * (N + 1) * (N + 2) * (N + 3);
        __int128 part2 = 3 + 2 * N;
        __int128 poly  = 1 + 36 * N - 69 * N2 + 45 * N4 + 18 * N5 + 2 * N6;

        __int128 numer = part1 * part2 * poly;
        return (seq_t)(numer / 3960);
    }

    /* ---------- 128‑bit seq_t or arbitrary‑precision seq_t ---------------- */
    else {
        seq_t N   = (seq_t)n;
        seq_t N2  = N * N;
        seq_t N4  = N2 * N2;
        seq_t N5  = N4 * N;
        seq_t N6  = N5 * N;

        seq_t part1 = N * (N + 1) * (N + 2) * (N + 3);
        seq_t part2 = 3 + 2 * N;
        seq_t poly  = 1 + 36 * N - 69 * N2 + 45 * N4 + 18 * N5 + 2 * N6;

        seq_t numer = part1 * part2 * poly;
        return numer / 3960;
    }
}
