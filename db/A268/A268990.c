/*
 * A268990  Number of n X 3 binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two not
 *          more than once.
 * https://oeis.org/A268990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 31*a(n-2) + 30*a(n-3) - 9*a(n-4)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=7, a(2)=35, a(3)=174, a(4)=849               */

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)35;
    if (n == 3) return (seq_t)174;
    if (n == 4) return (seq_t)849;

    /* For n >= 5, compute iteratively from the recurrence. */
    seq_t v0 = (seq_t)7;   /* a(1)   */
    seq_t v1 = (seq_t)35;  /* a(2)   */
    seq_t v2 = (seq_t)174; /* a(3)   */
    seq_t v3 = (seq_t)849; /* a(4)   */

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t v4 = (seq_t)10 * v3
                 - (seq_t)31 * v2
                 + (seq_t)30 * v1
                 - (seq_t)9  * v0;
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
    }
    return v3;  /* v3 holds a(n) after the loop */
}
