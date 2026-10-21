/*
 * A259589  Numerators of the other-side convergents to e.
 * https://oeis.org/A259589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = p(n) + p(n-1)
 *
 * where p(i) denotes the numerator of the i‑th convergent of e,
 * obtained from the standard recurrence
 *
 *     p(-2) = 0,   p(-1) = 1
 *     p(i)   = a(i) * p(i-1) + p(i-2)
 *
 * with a(i) the i‑th partial quotient of e:
 *
 *     a(0) = 2
 *     a(i) = 1         if i % 3 == 0 or i % 3 == 1
 *     a(i) = 2 * ((i+1)/3)  if i % 3 == 2
 *
 * The algorithm iterates once over the indices up to n, keeping
 * only the two most recent numerators.  It works for any
 * integer type seq_t, including the 8/16/32/64/128‑bit unsigned
 * integers and arbitrary‑precision integer classes.
 */

seq_t a(idx_t n)
{
    /* previous two numerators of the standard convergents   */
    seq_t p_prev2 = 0;   /* p(-2) */
    seq_t p_prev1 = 1;   /* p(-1) */

    seq_t result = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        /* partial quotient a(i) for e */
        seq_t ai;
        if (i == 0)
        {
            ai = 2;
        }
        else
        {
            idx_t k = (i + 1) / 3;          /* integer division (k >= 0) */
            if ((i % 3) == 0 || (i % 3) == 1)
                ai = 1;
            else                            /* i % 3 == 2 */
                ai = (seq_t)(2 * k);
        }

        /* compute the next convergent numerator */
        seq_t p_i = ai * p_prev1 + p_prev2;

        /* if this is the requested index, assemble the result
           using p(i) + p(i-1) */
        if (i == n)
        {
            result = p_i + p_prev1;   /* p(n-1) is p_prev1 */
            break;
        }

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_i;
    }

    return result;
}
