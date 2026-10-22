/*
 * A383313  Expansion of e.g.f. exp(-x/2) / (1-2*x)^(1/4).
 * https://oeis.org/A383313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): a(0)=1, a(1)=0,
   a(n) = (n-1)*(2*a(n-1) + a(n-2))   for n > 1   */

/* Compute the n-th term of OEIS A383313.  Works for any
   seq_t and idx_t defined by the build (built‑in integer types
   or arbitrary‑precision integer classes).  The function
   runs in O(n) time and uses O(1) additional memory. */
seq_t a(idx_t n)
{
    /* Handle the two initial values directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;

    /* a0 = a(n-2), a1 = a(n-1) during the loop */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)0;   /* a(1) */

    /* Iterate from n = 2 up to the requested index */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* Compute 2*a1 + a0 */
        seq_t temp = (seq_t)2 * a1 + a0;

        /* Multiply by (i-1) to get a(i) */
        seq_t ai = (seq_t)(i - 1) * temp;

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = ai;
    }

    return a1;
}
