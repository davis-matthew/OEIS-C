/*
 * A317255  a(n) = 149836681069944461 + (n-1)*1723457117682300.
 * https://oeis.org/A317255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 149836681069944461 + (n-1)*1723457117682300
 * Range:     u8 none, u16 none, u32 none, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The first term (n = 1) and the common difference. */
    static const seq_t first = 149836681069944461ULL;
    static const seq_t step  = 1723457117682300ULL;

    /* For n = 0 the sequence is undefined; return 0 to avoid UB. */
    if (n == 0) return 0;

    return first + (seq_t)(n - 1) * step;
}
