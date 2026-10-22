/*
 * A357266  Number of n-node tournaments that have exactly five circular
 *          triads.
 * https://oeis.org/A357266
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A357266: Number of n-node tournaments that have exactly five circular triads
   Formula (Kadane):
   a(n) = n! * [ (1/5)*(n-4) + (14/3)*(n-5) + 8*(n-6)*I(n>5) + (7/9)*(n-6)*(n-7)*I(n>5)
               + (10/3)*(n-7)*(n-8)*I(n>6) + (5/18)*(n-8)*(n-9)*(n-10)*I(n>7)
               + (1/162)*(n-9)*(n-10)*(n-11)*(n-12)*I(n>8)
               + (1/29160)*(n-10)*(n-11)*(n-12)*(n-13)*(n-14)*I(n>9) ]
   where I(p) is 1 if p is true, else 0. */

#include <stdbool.h>
#include "../utils/gcd.h"

/* Helper: divide n! by denom, using exact gcd cancellation to avoid overflow. */
static seq_t fact_div(idx_t n, seq_t denom)
{
    seq_t res = 1;
    seq_t d   = denom;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = (seq_t)i;
        seq_t g   = gcd(tmp, d);
        if (g > 1) {
            tmp /= g;
            d   /= g;
        }
        res *= tmp;
    }
    /* d should be 1 at this point; if not, denom does not divide n! (shouldn't happen). */
    return res;
}

seq_t a(idx_t n)
{
    if (n < 5) return 0;              /* the sequence is undefined for n < 5 */

    /* Compute n! once; for 64‑bit seq_t it is safe for all n that will fit in the type. */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t result = 0;

    /* (1/5)*(n-4)*n! */
    result += fact_div(n, (seq_t)5) * (seq_t)(n - 4);

    /* (14/3)*(n-5)*n! */
    result += fact_div(n, (seq_t)3) * (seq_t)14 * (seq_t)(n - 5);

    if (n > 5) {
        /* 8*(n-6)*n! */
        result += fact * (seq_t)8 * (seq_t)(n - 6);

        /* (7/9)*(n-6)*(n-7)*n! */
        result += fact_div(n, (seq_t)9) * (seq_t)7 *
                  (seq_t)(n - 6) * (seq_t)(n - 7);
    }

    if (n > 6) {
        /* (10/3)*(n-7)*(n-8)*n! */
        result += fact_div(n, (seq_t)3) * (seq_t)10 *
                  (seq_t)(n - 7) * (seq_t)(n - 8);
    }

    if (n > 7) {
        /* (5/18)*(n-8)*(n-9)*(n-10)*n! */
        result += fact_div(n, (seq_t)18) * (seq_t)5 *
                  (seq_t)(n - 8) * (seq_t)(n - 9) * (seq_t)(n - 10);
    }

    if (n > 8) {
        /* (1/162)*(n-9)*(n-10)*(n-11)*(n-12)*n! */
        result += fact_div(n, (seq_t)162) *
                  (seq_t)(n - 9) * (seq_t)(n - 10) *
                  (seq_t)(n - 11) * (seq_t)(n - 12);
    }

    if (n > 9) {
        /* (1/29160)*(n-10)*(n-11)*(n-12)*(n-13)*(n-14)*n! */
        result += fact_div(n, (seq_t)29160) *
                  (seq_t)(n - 10) * (seq_t)(n - 11) *
                  (seq_t)(n - 12) * (seq_t)(n - 13) *
                  (seq_t)(n - 14);
    }

    return result;
}
