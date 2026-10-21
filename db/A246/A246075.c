/*
 * A246075  Paradigm shift sequence for a (-3,5) production scheme with
 *          replacement.
 * https://oeis.org/A246075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * a(n-7) for all n ≥ 14. Initial terms: a(n) = n for 1
 *            ≤ n ≤ 13
 * Range:     u8 n <= 42, u16 n <= 98, u32 n <= 210, u64 n <= 434,
 *            u128 n <= 882, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for any n ≥ 1.  */
seq_t a(idx_t n)
{
    /* n ≤ 13: direct return */
    if (n <= 13)
        return (seq_t)n;

    /* Reduce n by steps of 7 until it falls into the initial range. */
    idx_t m = n;      /* current index in the recurrence chain */
    idx_t k = 0;      /* number of times we multiplied by 2 */

    while (m > 13) {  /* keep looping while we haven't hit the base case */
        m -= 7;
        ++k;
    }

    /* m is now in 1..13, so the value is simply m itself. */
    seq_t val = (seq_t)m;

    /* Multiply by 2^k using left shifts: val <<= k. */
    while (k--) {
        val <<= 1;    /* equivalent to val *= 2 */
    }

    return val;
}
