/*
 * A295909  Number of (not necessarily maximal) cliques in the n X n black
 *          bishop graph.
 * https://oeis.org/A295909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((-2)^(n + 1) + (-1)^n + 19*2^(n + 1) - 6*n*(n + 4) -
 *            25)/12
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 26, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 2^(n+1) */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n + 1; ++i) {
        pow2 *= 2;
    }

    /* (n+1 even) ? 20*pow2 : 18*pow2  */
    seq_t term_sum = pow2;
    if (((n + 1) & 1) == 0) {
        term_sum *= 20;
    } else {
        term_sum *= 18;
    }

    /* numerator */
    seq_t num = term_sum;
    if ((n & 1) == 0) {
        num += 1;
    } else {
        num -= 1;
    }
    seq_t sub = n;
    sub *= (n + 4);
    sub *= 6;
    num -= sub;
    num -= 25;

    return num / 12;
}
