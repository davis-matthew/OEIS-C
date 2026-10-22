/*
 * A333718  a(n) = L(8*n+4)/7, where L = A000032 (the Lucas sequence).
 * https://oeis.org/A333718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 47*a(n-1) - a(n-2); a(0) = 1, a(1) = 46
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)46;

    seq_t prev = (seq_t)1;   /* a(0) */
    seq_t curr = (seq_t)46;  /* a(1) */
    seq_t next;

    /* Iteratively compute up to a(n) */
    for (idx_t i = 2; i <= n; ++i) {
        next = ((seq_t)47) * curr - prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
