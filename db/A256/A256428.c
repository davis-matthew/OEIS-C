/*
 * A256428  G.f.: x^2*(1-2*x)/(1-8*x+22*x^2-26*x^3+14*x^4-5*x^5+x^6).
 * https://oeis.org/A256428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A256428: linear recurrence derived from the generating
   function x^2*(1-2*x)/(1-8*x+22*x^2-26*x^3+14*x^4-5*x^5+x^6)
   a(0)=0, a(1)=0, a(2)=1, a(3)=6, a(4)=26, a(5)=102
   a(n) = 8*a(n-1) - 22*a(n-2) + 26*a(n-3) - 14*a(n-4) + 5*a(n-5) - a(n-6)
   for all n ≥ 6 */

seq_t a(idx_t n)
{
    /* handle the first six terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)26;
    if (n == 5) return (seq_t)102;

    /* array of the last six computed values:
       [a(k-6), a(k-5), a(k-4), a(k-3), a(k-2), a(k-1)] */
    seq_t prev[6] = {0, 0, 1, 6, 26, 102};
    seq_t val;

    for (idx_t k = 6; k <= n; ++k) {
        /* recurrence: a(k) = 8*a(k-1) - 22*a(k-2) + 26*a(k-3) - 14*a(k-4)
                           + 5*a(k-5) - a(k-6) */
        val = (seq_t)8  * prev[5];
        val = val - (seq_t)22 * prev[4];
        val = val + (seq_t)26 * prev[3];
        val = val - (seq_t)14 * prev[2];
        val = val + (seq_t)5  * prev[1];
        val = val -                  prev[0];

        /* shift the window for the next iteration */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = val;
    }

    return prev[5];
}
