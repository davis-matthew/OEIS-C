/*
 * A247061  Dynamic Betting Game D(n,5,1).
 * https://oeis.org/A247061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1)=1, a(2)=8, a(3)=16, a(4)=17;
   a(n) = a(n-1) + a(n-3) - a(n-4)   for n ≥ 5 */

seq_t a(idx_t n)
{
    /* Handle the known initial values explicitly. */
    if (n == 0) return (seq_t)0;   /* not used by the OEIS offset */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)16;
    if (n == 4) return (seq_t)17;

    /* Iteratively apply the recurrence, keeping only the last
       four values needed.  This works for both integral types
       (in the C build) and for arbitrary-precision classes
       (in the C++ build). */
    seq_t a1 = 1,       /* a(1) */
           a2 = 8,       /* a(2) */
           a3 = 16,      /* a(3) */
           a4 = 17;      /* a(4) */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i)
    {
        next = a4 + a2 - a1;   /* a(i) = a(i-1) + a(i-3) - a(i-4) */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
