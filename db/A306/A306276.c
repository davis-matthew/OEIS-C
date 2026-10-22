/*
 * A306276  a(0) = a(1) = a(2) = a(3) = 1; thereafter a(n) = a(n-2) + a(n-3) +
 *          a(n-4).
 * https://oeis.org/A306276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 59, u64 n <= 117,
 *            u128 n <= 233, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A306276:
   a(0) = a(1) = a(2) = a(3) = 1;
   a(n) = a(n-2) + a(n-3) + a(n-4)  for n >= 4 */

/* No external dependencies beyond the types seq_t and idx_t. */
seq_t a(idx_t n)
{
    /* For the first four indices the value is always 1. */
    if (n < 4) {
        return (seq_t)1;
    }

    /* Maintain the last four computed terms:
       a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1). */
    seq_t a0 = (seq_t)1; /* a(0)  */
    seq_t a1 = (seq_t)1; /* a(1)  */
    seq_t a2 = (seq_t)1; /* a(2)  */
    seq_t a3 = (seq_t)1; /* a(3)  */

    /* Iterate from 4 up to n, updating the sliding window. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a0 + a1; /* sum a(n-4) + a(n-3)   */
        next += a2;           /* add a(n-2) to get a(n) */
        /* Shift the window: discard a(n-4), move the rest up. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
