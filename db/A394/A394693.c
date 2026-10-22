/*
 * A394693  a(n) = (n!)^6 * Sum_{1<=i<=j<=k<=n} 1/(i*j*k)^2.
 * https://oeis.org/A394693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^6 * (H(n,2)^3 + 3*H(n,2)*H(n,4) + 2*H(n,6)) / 6
 *            Recurrence (n > 2): a(0) = 0 a(1) = 1 a(2) = 85 a(n) =
 *            n^2*(3*n^4-6*n^3+7*n^2-4*n+1)*a(n-1) -
 *            n^2*(n-1)^8*(3*n^2-6*n+5)*a(n-2) + n^2*(n-1)^8*(n-2)^8*a(n-3) +
 *            ((n-1)!)^6
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* compute power: base^exp, exp <= 10 for this sequence */
static seq_t pow_seq(seq_t base, unsigned exp)
{
    seq_t result = 1;
    for (unsigned e = 0; e < exp; ++e)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)85;

    /* Initial history */
    seq_t a_prev3 = (seq_t)0;   /* a(0) */
    seq_t a_prev2 = (seq_t)1;   /* a(1) */
    seq_t a_prev1 = (seq_t)85;  /* a(2) */

    /* factorials for (i-1)!^6; start with (2)!^6 for i=3 */
    seq_t fact_cur = (seq_t)1;          /* current factorial (i-1)! */
    for (idx_t t = 1; t <= 2; ++t)
        fact_cur *= (seq_t)t;
    seq_t fact6_cur = pow_seq(fact_cur, 6);  /* (i-1)!^6 */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t ni  = (seq_t)i;
        seq_t n2  = ni * ni;
        seq_t n3  = n2 * ni;
        seq_t n4  = n3 * ni;

        /* coefficient of a(n-1) */
        seq_t poly   = 3 * n4 - 6 * n3 + 7 * n2 - 4 * ni + 1;
        seq_t coeff1 = n2 * poly;
        seq_t term1  = coeff1 * a_prev1;

        /* coefficient of a(n-2) */
        seq_t n1    = ni - 1;
        seq_t n1_8  = pow_seq(n1, 8);
        seq_t H     = 3 * n2 - 6 * ni + 5;
        seq_t coeff2 = n2 * n1_8 * H;
        seq_t term2  = coeff2 * a_prev2;

        /* coefficient of a(n-3) */
        seq_t n2_8   = pow_seq(ni - 2, 8);
        seq_t coeff3 = n2 * n1_8 * n2_8;
        seq_t term3  = coeff3 * a_prev3;

        /* compute a(n) */
        seq_t a_next = term1 - term2 + term3 + fact6_cur;

        /* shift history */
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_next;

        /* update factorials for next iteration */
        fact_cur *= ni;                     /* becomes i! for next i+1 */
        fact6_cur *= pow_seq(ni, 6);        /* now (i)!^6 */
    }

    return a_prev1;
}
