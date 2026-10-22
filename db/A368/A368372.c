/*
 * A368372  a(n) = numerator of AM(n)-HM(n), where AM(n) and HM(n) are the
 *          arithmetic and harmonic means of the first n positive integers.
 * https://oeis.org/A368372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of (AM(n)-HM(n)) * where AM(n) = (n+1)/2 and
 *            HM(n) = n / H(n), H(n)=1+1/2+...+1/n * The result is the
 *            numerator of ( (n+1)/2 - n/H(n) ) after the fraction * has been
 *            fully reduced
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static size_t cached_n = 0;          /* largest index already computed */
static size_t capacity   = 0;        /* allocated size of the array (>= cached_n+1) */
static seq_t *a_val      = NULL;     /* a(i) for all i <= cached_n */

/* Numerator and denominator of the current harmonic mean H_n = p/q */
static seq_t H_num = 0;              /* p */
static seq_t H_den = 1;              /* q */

/* Ensure capacity for indices up to n (1‑based). */
static void ensure_capacity(idx_t n)
{
    if (capacity >= (size_t)n + 1)
        return;
    size_t new_cap = capacity ? capacity : 16;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;
    a_val = (seq_t *)realloc(a_val, new_cap * sizeof(seq_t));
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)         /* offset is 1; behaviour below 1 is irrelevant */
        return 0;

    ensure_capacity(n);

    for (idx_t i = cached_n + 1; i <= n; ++i)
    {
        if (i == 1)
        {
            /* H(1) = 1, AM(1)-HM(1) = 0 */
            H_num = 1;
            H_den = 1;
            a_val[1] = 0;
        }
        else
        {
            /* Update the harmonic sum H_i = H_{i-1} + 1/i
             *   H_{i-1} = H_num / H_den
             *   H_i     = (H_num*i + H_den) / (H_den*i)  reduced
             */
            seq_t new_num = H_num * (seq_t)i + H_den;
            seq_t new_den = H_den * (seq_t)i;
            seq_t g = gcd(new_num, new_den);
            new_num /= g;
            new_den /= g;
            H_num = new_num;
            H_den = new_den;

            /* Difference fraction before reduction:
             *   diff = (i+1)/2 - i / (H_num/H_den)
             *        = [(i+1)*H_num - 2*i*H_den] / [2*H_num]
             */
            seq_t term1 = (seq_t)(i + 1) * H_num;     /* (i+1)*H_num */
            seq_t term2 = (seq_t)2 * (seq_t)i * H_den;/* 2*i*H_den   */
            seq_t diff_num = term1 - term2;
            seq_t diff_den = (seq_t)2 * H_num;
            seq_t g2 = gcd(diff_num, diff_den);
            diff_num /= g2;        /* reduced numerator */
            a_val[i] = diff_num;   /* store numerator of a(i) */
        }
    }
    cached_n = (size_t)n;
    return a_val[n];
}
