/*
 * A286307  a(n) is the numerator of r(n), where r(n) = r(n-1) +
 *          r(n-2)/(2*(n-1)) with r(0) = 0, r(1) = 1.
 * https://oeis.org/A286307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the numerator of r(n) with r(0)=0, r(1)=1,
 *            r(n)=r(n-1)+r(n-2)/(2*(n-1)) (n≥2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the generic gcd implementation from the utilities. */
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static memoisation storage */
    static seq_t *num = NULL;          /* numerators  */
    static seq_t *den = NULL;          /* denominators */
    static size_t cap = 0;             /* allocated size   */
    static idx_t computed_max = 1;     /* highest index computed so far */

    /* make sure arrays are large enough */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp_num = (seq_t *)realloc(num, new_cap * sizeof(seq_t));
        seq_t *tmp_den = (seq_t *)realloc(den, new_cap * sizeof(seq_t));
        if (!tmp_num || !tmp_den) {
            if (tmp_num) free(tmp_num);
            if (tmp_den) free(tmp_den);
            return (seq_t)0;          /* allocation failed – unlikely */
        }
        num = tmp_num;  den = tmp_den;  cap = new_cap;
    }

    /* initialise base cases if not already done */
    if (cap > 0) {
        num[0] = (seq_t)0;  den[0] = (seq_t)1;
    }
    if (cap > 1) {
        num[1] = (seq_t)1;  den[1] = (seq_t)1;
    }

    /* compute missing terms up to n */
    if (n > computed_max) {
        idx_t start = (computed_max >= 2) ? (computed_max + 1) : 2;
        for (idx_t i = start; i <= n; ++i) {
            seq_t p1 = num[i - 1];
            seq_t q1 = den[i - 1];
            seq_t p2 = num[i - 2];
            seq_t q2 = den[i - 2];
            seq_t k  = (seq_t)2 * (seq_t)(i - 1);   /* 2*(i-1) */

            /* denominator of the second term: q2 * k */
            seq_t den2 = q2 * k;

            /* lcm of q1 and den2 */
            seq_t g   = gcd(q1, den2);
            seq_t q1_red  = q1 / g;
            seq_t den2_red = den2 / g;

            /* combine numerators over common denominator */
            seq_t part1 = p1 * den2_red;
            seq_t part2 = p2 * q1_red;
            seq_t new_num = part1 + part2;
            seq_t new_den = q1_red * den2;

            /* reduce the resulting fraction */
            seq_t g2 = gcd(new_num, new_den);
            new_num /= g2;
            new_den /= g2;

            num[i] = new_num;
            den[i] = new_den;
        }
        computed_max = n;
    }

    return num[n];
}
