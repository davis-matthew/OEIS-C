/*
 * A244279  Numerators of the n-th iteration of the alternating continued
 *          fraction of the positive integers, initiated with (1 + ...).
 * https://oeis.org/A244279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the fraction 1/(1 + 1/(2 - 1/(3 + 1/(4 - ...
 *            1/(n +/- 1)))), where the outermost + is for odd indices and the
 *            inner signs alternate. Computation uses an O(n) recurrence
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case: the innermost term 1/(n +/- 1) */
    seq_t p;           /* numerator of the current sub‑fraction */
    seq_t q;           /* denominator of the current sub‑fraction */

    if (n & 1UL)       /* n is odd → plus 1 */
        p = (seq_t)n + 1;
    else               /* n is even → minus 1 */
        p = (seq_t)n - 1;
    q = 1;

    /* Iterate downwards from n-1 to 1.  At step k we update
       r_k = k +/- 1/r_{k+1} giving the new fraction = (k*p +/- q)/p.
       The new denominator becomes p, and the new numerator becomes k*p +/- q. */
    for (idx_t k = n - 1; k >= 1; --k) {
        seq_t newp;
        if (k & 1UL)     /* k odd → plus */
            newp = (seq_t)k * p + q;
        else             /* k even → minus */
            newp = (seq_t)k * p - q;
        seq_t newq = p;
        p = newp;
        q = newq;
    }

    /* The final value of a(n) is 1/r_1 = q/p, so its numerator is q. */
    return q;
}
