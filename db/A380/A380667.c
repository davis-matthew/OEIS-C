/*
 * A380667  First differences of the Golay-Rudin-Shapiro sequence (A020985),
 *          divided by 2.
 * https://oeis.org/A380667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (A020985(n+1) - A020985(n)) / 2
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 9816 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the Rudin–Shapiro value (+1 or -1) for n. */
static inline seq_t rudine(idx_t n)
{
    uint64_t x = n & (n >> 1);        /* bits where two consecutive ones appear */
#if defined(__GNUC__) || defined(__clang__)
    int parity = (__builtin_popcountll(x) & 1);
#else
    /* Fallback: compute parity of popcount without large lookup tables */
    int parity = 0;
    while (x) {
        parity ^= (int)(x & 1);
        x >>= 1;
    }
#endif
    return (seq_t)(parity == 0 ? 1 : -1);
}

seq_t a(idx_t n)
{
    seq_t v_n  = rudine(n);
    seq_t v_np = rudine(n + 1);
    return (v_np - v_n) / (seq_t)2;
}
