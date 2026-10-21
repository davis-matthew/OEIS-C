/*
 * A255118  Number of n-length words on {0,1,2,3,4,5} in which 0 appears only
 *          in runs of length 2.
 * https://oeis.org/A255118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 5*a(n-3) for n ≥ 3, with initial values
 *            a(0)=1, a(1)=5, a(2)=26
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)26;

    /* Keep the last three values in a sliding window:
       a0 = a(n-3), a1 = a(n-2), a2 = a(n-1). */
    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)5; /* a(1) */
    seq_t a2 = (seq_t)26;/* a(2) */
    seq_t ai;            /* current value a(i) */

    for (idx_t i = 3; i <= n; ++i) {
        ai = (seq_t)5 * a2 + (seq_t)5 * a0; /* a(i) = 5*a(i-1)+5*a(i-3) */
        a0 = a1;        /* shift window for next iteration */
        a1 = a2;
        a2 = ai;
    }
    return a2; /* a(n) has been computed in a2 after the loop */
}
