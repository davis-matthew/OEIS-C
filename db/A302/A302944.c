/*
 * A302944  a(n) = 4*((2*n-1)*a(n-1) + (-1)^n*(n-2)!), with a(1) = 8, n > 1.
 * https://oeis.org/A302944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*((2*n-1)*a(n-1) + (-1)^n*(n-2)!), a(1)=8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out‑of‑bounds indices return 0 */

    seq_t res = (seq_t)8;                /* a(1) */
    if (n == 1) return res;

    seq_t fact = (seq_t)1;               /* (n-2)! : start with 0! = 1 for n = 2 */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t mult = ((seq_t)(2*i - 1)) * res;  /* (2*i-1)*a(i-1) */
        if (i & 1)                       /* odd i → subtract factorial */
            mult -= fact;
        else                            /* even i → add factorial   */
            mult += fact;
        res = (seq_t)4 * mult;           /* a(i) = 4*mult          */

        fact *= (i - 1);                 /* update factorial to (i-1)! for next step */
    }

    return res;
}
