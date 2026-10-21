/*
 * A276275  Padovan like sequence: a(n) = a(n-2) + a(n-3) for n>3, a(1)=2,
 *          a(2)=2, a(3)=0.
 * https://oeis.org/A276275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) for n > 3 Initial values: a(1) = 2, a(2)
 *            = 2, a(3) = 0 The function is fully iterative (O(n)) and works
 *            for any idx_t n ≥ 1. It handles arbitrary‑precision seq_t
 *            (integer or bigint) by using native addition operations and
 *            casting integer constants to seq_t
 * Range:     u8 n <= 20, u16 n <= 40, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)0;              /* not part of OEIS offset, but safe */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)0;

    /* t0 = a(n-3), t1 = a(n-2), t2 = a(n-1) for the current loop iteration */
    seq_t t0 = (seq_t)2;     /* a1 */
    seq_t t1 = (seq_t)2;     /* a2 */
    seq_t t2 = (seq_t)0;     /* a3 */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t ai = t0 + t1;  /* a(i-2) + a(i-3) */
        t0 = t1;             /* shift the window: new a(n-3) */
        t1 = t2;             /* new a(n-2) */
        t2 = ai;             /* new a(n-1) (which is a(i)) */
    }
    return t2;               /* a(n) */
}
