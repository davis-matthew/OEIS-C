/*
 * A383953  a(0) = 4, a(n) = 2*a(n-1) + (-1)^n.
 * https://oeis.org/A383953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 4, a(n) = 2*a(n-1) + (-1)^n. */
/* The recurrence is evaluated iteratively for every call; it works for
   any order of calls and for all values of n >= 0. */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)4;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        val = (val << 1);          /* 2 * a(i-1) */
        if ((i & 1) == 0)          /* even i: add 1 */
            val = val + (seq_t)1;
        else                       /* odd i: subtract 1 */
            val = val - (seq_t)1;
    }
    return val;
}
