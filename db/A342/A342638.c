/*
 * A342638  a(0) = 0, a(1) = 1; a(2*n) = a(n), a(2*n+1) = 9*a(n) + a(n+1).
 * https://oeis.org/A342638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 3070, u64 n <= 9978,
 *            u128 n <= 9978, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 1;
   a(2*n)   = a(n),
   a(2*n+1) = 9*a(n) + a(n+1). */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if ((n & 1) == 0) {
        return a(n >> 1);
    } else {
        seq_t low  = a(n >> 1);
        seq_t high = a((n >> 1) + 1);
        return 9 * low + high;
    }
}
