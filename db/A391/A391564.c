/*
 * A391564  Numerator of the constant term of the remainder obtained when
 *          Stern polynomial B(n,x) is divided by B(5,x).
 * https://oeis.org/A391564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac, look
 * Formula:   a(n) = numerator of the constant term of the remainder obtained
 *            when Stern polynomial B(n,x) is divided by B(5,x)
 * Range:     i8 n <= 510, i16 n <= 9080, i32 n <= 9080, i64 n <= 9080,
 *            i128 n <= 9080, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation follows the recursive definition of
   B(n,x):  B(1,x)=1, B(2n,x)=B(n,x), B(2n+1,x)=x*B(n,x)+B(n+1,x).
   The numerator is computed in lowest terms as a fraction with a
   power‑of‑two denominator.  For an even index the numerator changes
   sign.  This implements the exact recurrence used by the OEIS
   sequence A391564. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* dynamic storage */
static seq_t   *num = NULL;     /* rational numerator (in lowest terms) */
static uint32_t *expv = NULL;   /* exponent of the denominator 2^expv */
static bool   *have = NULL;     /* true if value has already been computed */
static size_t     cap = 0;       /* current capacity of the arrays */

/* Ensure the internal arrays have room for indices 0..n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n < cap) return;
    size_t newcap = cap ? cap : 2;
    while (newcap <= (size_t)n) newcap <<= 1;
    num   = (seq_t*)      realloc(num,   newcap * sizeof(seq_t));
    expv  = (uint32_t*)    realloc(expv,  newcap * sizeof(uint32_t));
    have  = (bool*)        realloc(have,  newcap * sizeof(bool));
    for (size_t i = cap; i < newcap; ++i) have[i] = false;
    cap = newcap;
}

/* Recursive computation of the reduced fraction B(n,-1/2) */
static void compute(idx_t n)
{
    if (have[n]) return;

    if (n == 1) {                 /* base case */
        num[n]  = 1;
        expv[n] = 0;
        have[n] = true;
        return;
    }

    if ((n & 1) == 0) {           /* n is even : B(2m) = B(m) */
        idx_t m = n >> 1;
        compute(m);
        /* flip sign, increase denominator exponent by 1 */
        seq_t new_num = -num[m];
        uint32_t new_exp = expv[m] + 1;
        /* cancel common factors of 2 to keep the numerator odd */
        while (new_exp && ((new_num & 1) == 0)) {
            new_num >>= 1;
            --new_exp;
        }
        num[n]   = new_num;
        expv[n]  = new_exp;
        have[n]  = true;
    } else {                      /* n is odd : B(2m+1) = x*B(m)+B(m+1) */
        idx_t a = n >> 1;         /* a = floor(n/2) = (n-1)/2   */
        idx_t b = a + 1;          /* b = a+1                    */
        compute(a);
        compute(b);
        uint32_t e1 = expv[a];
        uint32_t e2 = expv[b];
        uint32_t m  = e1 > e2 ? e1 : e2;          /* common exponent   */
        seq_t s1 = num[a] * ((seq_t)1 << (m - e1));
        seq_t s2 = num[b] * ((seq_t)1 << (m - e2));
        seq_t new_num = s1 + s2;
        uint32_t new_exp = m;
        /* cancel obvious factors of 2 */
        while (new_exp && ((new_num & 1) == 0)) {
            new_num >>= 1;
            --new_exp;
        }
        num[n]   = new_num;
        expv[n]  = new_exp;
        have[n]  = true;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for offset, safe */
    ensure_capacity(n);
    compute(n);
    return num[n];
}
