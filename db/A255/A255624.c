/*
 * A255624  Number of length n+2 0..2 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/40320)*n^8 + (13/10080)*n^7 + (59/2880)*n^6 +
 *            (61/360)*n^5 + (5167/5760)*n^4 + (901/1440)*n^3 +
 *            (17077/3360)*n^2 + (3977/280)*n + 6
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Integer coefficients of the numerator polynomial
       after removing the common denominator 40320.  */
    static const seq_t COEF[9] = {
        (seq_t)241920,  /* c0 = 6 * 40320 */
        (seq_t)572688,  /* c1 = 3977 * (40320/280)   */
        (seq_t)204924,  /* c2 = 17077 * (40320/3360) */
        (seq_t)25228,   /* c3 = 901 * (40320/1440)   */
        (seq_t)36169,   /* c4 = 5167 * (40320/5760)  */
        (seq_t)6832,    /* c5 = 61 * (40320/360)     */
        (seq_t)826,     /* c6 = 59 * (40320/2880)    */
        (seq_t)52,      /* c7 = 13 * (40320/10080)   */
        (seq_t)1        /* c8 = 1                     */
    };

    seq_t N = (seq_t)n;
    seq_t res = COEF[8];          /* start with highest coefficient */
    for (int i = 7; i >= 0; --i) {
        res = res * N + COEF[i];
    }
    return res / (seq_t)40320;    /* divide by the common denominator */
}
