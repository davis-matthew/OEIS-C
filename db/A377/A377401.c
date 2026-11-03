/*
 * A377401  a(n) = denominator((1/n!)*Sum_{k=1..n} 1/(2*k)).
 * https://oeis.org/A377401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((1/n!)*Sum_{k=1..n} 1/(2*k))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t *fact   = NULL;
static seq_t *harm_num = NULL;
static seq_t *harm_den = NULL;
static seq_t *res_arr   = NULL;
static idx_t  max_comp   = 0;   /* largest n computed so far */

seq_t a(idx_t n)
{
    /* First call: allocate and build up to n */
    if (res_arr == NULL) {
        size_t sz = (size_t)(n + 1);
        fact     = mem_new(seq_t, sz);
        harm_num = mem_new(seq_t, sz);
        harm_den = mem_new(seq_t, sz);
        res_arr  = mem_new(seq_t, sz);

        fact[0]     = (seq_t)1;
        harm_num[0] = (seq_t)0;
        harm_den[0] = (seq_t)1;
        res_arr[0]  = (seq_t)1;          /* a(0) */

        for (idx_t k = 1; k <= n; ++k) {
            /* factorial */
            fact[k] = fact[k-1] * (seq_t)k;

            /* add 1/k to the harmonic sum */
            seq_t num = harm_num[k-1] * (seq_t)k + harm_den[k-1];
            seq_t den = harm_den[k-1] * (seq_t)k;
            seq_t g   = gcd(num, den);
            harm_num[k] = num / g;
            harm_den[k] = den / g;

            /* denominator of (H_k)/(2*k!) */
            seq_t denom = (harm_den[k] * (seq_t)2) * fact[k];
            seq_t g2    = gcd(harm_num[k], denom);
            res_arr[k]  = denom / g2;
        }
        max_comp = n;
    }
    /* Increase cache when a larger n is requested */
    else if (n > max_comp) {
        size_t new_sz = (size_t)(n + 1);
        fact   = mem_resize(seq_t, fact, new_sz);
        harm_num = mem_resize(seq_t, harm_num, new_sz);
        harm_den = mem_resize(seq_t, harm_den, new_sz);
        res_arr  = mem_resize(seq_t, res_arr, new_sz);

        for (idx_t k = max_comp + 1; k <= n; ++k) {
            fact[k] = fact[k-1] * (seq_t)k;

            seq_t num = harm_num[k-1] * (seq_t)k + harm_den[k-1];
            seq_t den = harm_den[k-1] * (seq_t)k;
            seq_t g   = gcd(num, den);
            harm_num[k] = num / g;
            harm_den[k] = den / g;

            seq_t denom = (harm_den[k] * (seq_t)2) * fact[k];
            seq_t g2    = gcd(harm_num[k], denom);
            res_arr[k]  = denom / g2;
        }
        max_comp = n;
    }
    return res_arr[n];
}
