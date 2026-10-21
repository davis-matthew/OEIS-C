/*
 * A287744  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 342", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A287744:
   a(0)=1, a(1)=1;
   for even n≥2: a(n)=2*a(n-2)+((n/2)%2==0?1:0);
   for odd  n≥1 : a(n)=a(n-1);
*/
seq_t a(idx_t n)
{
    if (n & 1U) n -= 1U;            /* odd indices repeat the previous even value */
    idx_t m = n >> 1;               /* m = n/2, so we compute a(2*m) */
    seq_t val = 1;                  /* a(0) = 1 */

    for (idx_t i = 1; i <= m; ++i) {
        val = val * 2;              /* multiply by 2 each step */
        if ((i & 1U) == 0U)         /* add 1 when the step index i is even */
            val += 1;
    }
    return val;
}
