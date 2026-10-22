/*
 * A323978  Expansion of determinant of 24 X 24 matrix M_{u,v} =
 *          q^(maj(u*v^-1)) where u, v in S_4 and maj = MacMahon's major index
 *          (cf. A008302).
 * https://oeis.org/A323978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of q^n in (1-q^2)^12 (1-q^3)^16 (1-q^4)^18
 * Range:     i8 n <= 3, i16 n <= 12, i32 n <= 38, i64 n <= 144, i128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_DEGREE 144

/* compute binomial coefficient C(e,k) for small e<=18 */
static seq_t binom_calc(int e, int k) {
    if (k == 0 || k == e) return (seq_t)1;
    if (k > e - k) k = e - k;
    seq_t res = (seq_t)1;
    for (int i = 1; i <= k; ++i) {
        res = res * ((seq_t)(e - k + i));
        res = res / ((seq_t)i);      /* exact division */
    }
    return res;
}

seq_t a(idx_t n) {
    static int init = 0;
    static seq_t coeff[MAX_DEGREE + 1];
    if (!init) {
        /* initialise coeff array */
        for (int i = 0; i <= MAX_DEGREE; ++i) coeff[i] = 0;
        coeff[0] = 1;
        int current_max = 0;
        const int bases[] = {2, 3, 4};
        const int exps[]  = {12, 16, 18};
        for (int step = 0; step < 3; ++step) {
            int base = bases[step];
            int exp  = exps[step];
            int new_max = current_max + base * exp;
            seq_t newcoeff[MAX_DEGREE + 1];
            for (int i = 0; i <= new_max; ++i) newcoeff[i] = 0;
            for (int a = 0; a <= current_max; ++a) {
                seq_t coeff_a = coeff[a];
                if (coeff_a == 0) continue;
                for (int k = 0; k <= exp; ++k) {
                    int idx = a + base * k;
                    seq_t term = coeff_a * binom_calc(exp, k);
                    if (k & 1) term = -term;   /* (-1)^k factor */
                    newcoeff[idx] += term;
                }
            }
            for (int i = 0; i <= new_max; ++i) coeff[i] = newcoeff[i];
            current_max = new_max;
        }
        init = 1;
    }
    if (n <= (idx_t)MAX_DEGREE) {
        return coeff[(size_t)n];
    }
    return (seq_t)0;
}
