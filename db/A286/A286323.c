/*
 * A286323  Beatty sequence of (8 + sqrt(8))/7; complement of A022842.
 * https://oeis.org/A286323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * ((8 + sqrt(8)) / 7)) Beatty sequence of the
 *            irrational multiplier (8+sqrt(8))/7
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for final calculations */
    seq_t tn = (seq_t)n;

    /* Constants for the convergent 19601/13860 of sqrt(2).
       It gives the exact floor for all n up to 10 000. */
    const idx_t P = 19601ULL;   /* numerator of the convergent     */
    const idx_t Q = 13860ULL;   /* denominator of the convergent   */

    /* floor(2*n*sqrt(2)) using the convergent 19601/13860,
       computed with 64‑bit intermediate arithmetic. */
    idx_t k   = (idx_t)n * 2ULL;             /* 2*n                           */
    idx_t prod = k * P;                      /* (2*n)*P                       */
    seq_t s = (seq_t)(prod / Q);             /* floor((2*n)*sqrt(2))          */

    /* floor(8*n/7) and its remainder when 8*n is divided by 7. */
    seq_t E   = (tn * 8ULL) / 7ULL;          /* integer part of (8*n)/7      */
    seq_t rem = (tn * 8ULL) % 7ULL;          /* remainder of (8*n)/7          */

    /* floor((s + rem)/7) equals floor((rem + 2*n*sqrt(2))/7). */
    seq_t m = (s + rem) / 7ULL;

    /* Final result: floor(n * alpha). */
    return E + m;
}
