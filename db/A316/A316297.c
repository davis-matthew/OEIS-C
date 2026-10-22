/*
 * A316297  a(n) = n! times the denominator of the n-th harmonic number H(n).
 * https://oeis.org/A316297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * denominator(H(n)) where H(n) = sum_{i=1}^n 1/i
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1 */
    if (n < 1)
        return 0;

    /* ---------- static cache for intermediate values ----------
       num[i] : numerator of the reduced H(i)
       den[i] : denominator of the reduced H(i)
       fact[i] : i!                                 */
    static seq_t *num = NULL;
    static seq_t *den = NULL;
    static seq_t *fact = NULL;
    static size_t len = 0;   /* maximum i already cached (>=1) */
    static size_t cap = 0;   /* allocated size (index 0 unused)   */

    size_t target = (size_t)n;          /* target index          */

    /* ----- grow the cache if required ----- */
    if (target + 1 > cap) {              /* need indices 0..target */
        size_t new_cap = target + 1;     /* one more for index 0 */
        seq_t *tmp_num  = (seq_t *)realloc(num,  new_cap * sizeof(seq_t));
        seq_t *tmp_den  = (seq_t *)realloc(den,  new_cap * sizeof(seq_t));
        seq_t *tmp_fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!tmp_num || !tmp_den || !tmp_fact) {
            abort();   /* allocation failure */
        }
        num  = tmp_num;
        den  = tmp_den;
        fact = tmp_fact;
        cap  = new_cap;
    }

    /* ----- initialise first entry if cache empty ----- */
    if (len == 0) {
        /* indices 0 are unused; set i = 1 explicitly */
        num[1]   = (seq_t)1;
        den[1]   = (seq_t)1;
        fact[1]  = (seq_t)1;
        len = 1;
    }

    /* ----- extend the cache up to target ----- */
    for (size_t i = len + 1; i <= target; ++i) {
        seq_t i_seq = (seq_t)i;

        /* H(i-1) = num[i-1]/den[i-1]  =>  H(i) = H(i-1) + 1/i  */
        seq_t prev_num = num[i-1];
        seq_t prev_den = den[i-1];

        seq_t new_num = prev_num * i_seq + prev_den;  /* num(i-1)*i + denom(i-1) */
        seq_t new_den = prev_den * i_seq;             /* denom(i-1)*i           */

        /* Reduce the fraction */
        seq_t g = gcd(new_num, new_den);
        num[i] = new_num / g;
        den[i] = new_den / g;

        /* Factorial update */
        fact[i] = fact[i-1] * i_seq;
    }

    len = target;

    /* a(n) = n! * denominator(H(n)) */
    return fact[target] * den[target];
}
