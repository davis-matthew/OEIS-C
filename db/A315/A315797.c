/*
 * A315797  Coordination sequence Gal.6.588.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: if n % 8 == 0 : a(n) = 4·n if n % 8 == 1 :
 *            a(n) = 6·n if n % 8 == 2 : a(2) = 6, else a(n) = 12 + 28·(n/8)
 *            if n % 8 == 3 : a(n) = 6·n – 6 if n % 8 == 4 : a(n) = 4·n + 4 if
 *            n % 8 == 5 : a(n) = 6·n – 4 if n % 8 == 6 : a(n) = 28·((n/8) +
 *            1) if n % 8 == 7 : a(n) = 6·n – 6
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    idx_t r = n & 7ULL;                   /* n % 8 */
    idx_t k = n >> 3;                     /* n / 8 */

    switch (r)
    {
        case 0: /* n % 8 == 0 and n > 0 */
            return (seq_t)(4 * n);

        case 1: /* n % 8 == 1 */
            return (seq_t)(6 * n);

        case 2: /* n % 8 == 2 */
            return (n == 2) ? (seq_t)6 : (seq_t)(12 + 28 * k);

        case 3: /* n % 8 == 3 */
            return (seq_t)(6 * n - 6);

        case 4: /* n % 8 == 4 */
            return (seq_t)(4 * n + 4);

        case 5: /* n % 8 == 5 */
            return (seq_t)(6 * n - 4);

        case 6: /* n % 8 == 6 */
            return (seq_t)(28 * (k + 1));

        case 7: /* n % 8 == 7 */
            return (seq_t)(6 * n - 6);
    }

    /* unreachable, but needed to silence compiler warnings */
    return (seq_t)0;
}
