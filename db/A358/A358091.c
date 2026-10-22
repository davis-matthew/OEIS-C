/*
 * A358091  Triangle read by rows. Coefficients of the polynomials P(n, x) =
 *          2^(n-2)*(3*n-1)* hypergeometric([-3*n, 1 - n, -n + 4/3], [-n, -n +
 *          1/3], x). T(n, k) = [x^k] P(n, x).
 * https://oeis.org/A358091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 18, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: the sequence is the triangle of coefficients of
 * P(n,x) = 2^(n-2)*(3*n-1)*_3F_2([-3n,1-n,-n+4/3],[-n,-n+1/3];x).
 * The term a(N) is found by locating the Nth entry in row‑major order.
 */
#include <stdbool.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t) */
#include "../utils/gcd.h"    /* seq_t gcd(seq_t,a,seq_t) */

static seq_t abs_seq(seq_t x) { return x < (seq_t)0 ? -x : x; }

seq_t a(idx_t N)
{
    /* Find the row r such that r*(r+1)/2 >= N */
    idx_t r = 1;
    idx_t cum = 1;          /* cumulative count 1..r */
    while (cum < N) {
        ++r;
        cum += r;
    }

    /* Position within that row (1‑based) */
    idx_t before = (r - 1) * r / 2;
    idx_t pos = N - before;          /* 1 <= pos <= r */

    /* Base coefficient (k = 0) */
    seq_t cur;
    if (r == (idx_t)1) {
        cur = (seq_t)1;
    } else {
        seq_t pow2_part = pow2(r - (idx_t)2);
        cur = pow2_part * ((seq_t)3 * (seq_t)r - (seq_t)1);
    }

    if (pos == (idx_t)1) return cur; /* first element of the row */

    /* Compute subsequent coefficients up to position 'pos' */
    for (idx_t k = 0; k <= pos - 2; ++k) {           /* k = 0 … pos-2 */
        seq_t k_s  = (seq_t)k;
        seq_t r_s  = (seq_t)r;

        /* Parameters of the recurrence */
        seq_t X  = - (seq_t)3 * r_s + k_s;                          /* -3r + k */
        seq_t Y  = (seq_t)1 - r_s + k_s;                            /*  1-r + k */
        seq_t U  = - r_s + k_s;                                      /* -r + k */
        seq_t W  = k_s + (seq_t)1;                                   /* k + 1 */

        seq_t Zn = (seq_t)3 * k_s - ((seq_t)3 * r_s - (seq_t)4);     /* 3k - (3r-4) */
        seq_t Vn = (seq_t)3 * k_s - ((seq_t)3 * r_s - (seq_t)1);     /* 3k - (3r-1) */

        seq_t num = X * Y * Zn;                                      /* numerator */
        seq_t den = U * Vn * W;                                      /* denominator (positive) */

        /* Reduce denominator with current coefficient */
        seq_t cur_abs = abs_seq(cur);
        seq_t den_abs = abs_seq(den);
        seq_t g1 = gcd(cur_abs, den_abs);
        seq_t cur1 = cur / g1;
        seq_t den1 = den / g1;

        /* Reduce remaining denominator with numerator */
        seq_t num_abs = abs_seq(num);
        seq_t g2 = gcd(num_abs, den1);
        seq_t num1 = num / g2;
        seq_t den2 = den1 / g2;

        /* Final coefficient */
        if (den2 == (seq_t)1) {
            cur = cur1 * num1;
        } else {
            cur = (cur1 * num1) / den2;
        }
    }
    return cur;
}
