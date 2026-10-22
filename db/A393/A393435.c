/*
 * A393435  a(n) = (-1)^A033264(n).
 * https://oeis.org/A393435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = (-1)^A033264(n) – where A033264(n) counts the occurrences
 *            of the pattern '10' in the binary representation of n
 * Range:     i8 n <= 102, i16 n <= 102, i32 n <= 102, i64 n <= 102,
 *            i128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count the number of 10‑patterns in the binary expansion of n. */
    idx_t count = 0;
    if (n == 0)
        return (seq_t)1;  /* A033264(0) = 0 → (-1)^0 = 1 */

    idx_t prev  = n & 1;    /* least significant bit */
    idx_t rest  = n >> 1;   /* remaining bits, shifted right one place */

    while (rest > 0) {
        idx_t cur = rest & 1;          /* next higher bit */
        if (cur == 1 && prev == 0)     /* pattern 10 occurs */
            ++count;
        prev = cur;
        rest >>= 1;
    }

    /* Return (-1)^count.  Even count → 1, odd → -1. */
    return (count & 1) ? (seq_t)-1 : (seq_t)1;
}
