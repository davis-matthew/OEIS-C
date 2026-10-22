/*
 * A301741  a(n) = n! * [x^n] exp((n + 1)*x + x^2/2).
 * https://oeis.org/A301741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   mul_div_safe
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A301741
   Formula: a(n) = Sum_{k=0..⌊n/2⌋} n! * (n+1)^(n-2k) / (2^k * k! * (n-2k)!)
   Offset: 0
   (Implementation uses an iterative computation of the sum based on the
   recurrence between consecutive terms.)
*/

#include <stdint.h>
#include "../utils/mul_div_safe.h"

/* helper for small seq_t types: multiplication with 128‑bit guard */
static inline seq_t mul_safe(seq_t a, seq_t b)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)b;
        return (seq_t)prod;
    }
    return a * b;
}

/* integer power for small seq_t types */
static inline seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1ULL)
            result = mul_safe(result, base);
        base = mul_safe(base, base);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* base = n + 1 */
    seq_t base = (seq_t)(n + 1);

    /* term0 = (n+1)^n  (k = 0) */
    seq_t term = pow_uint(base, n);
    seq_t sum  = term;

    /* compute subsequent terms for k = 1 .. ⌊n/2⌋ */
    idx_t maxk = n / 2;           /* integer division truncates toward zero */

    for (idx_t k = 0; k < maxk; ++k) {
        /* n - 2*k and n - 2*k - 1 as seq_t */
        seq_t n2k    = (seq_t)n - (seq_t)(2 * k);
        seq_t numerator = n2k * (n2k - (seq_t)1);

        /* denominator part: 2 * (k+1) */
        seq_t denom1 = (seq_t)(k + 1);
        denom1 = mul_safe(denom1, (seq_t)2);

        /* full denominator: 2 * (k+1) * base^2 */
        seq_t denom2;
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 d = (unsigned __int128)denom1 *
                                  (unsigned __int128)base *
                                  (unsigned __int128)base;
            denom2 = (seq_t)d;
        } else {
            denom2 = denom1 * base * base;
        }

        /* update term: term = term * numerator / denom2 */
        term = mul_div_safe(term, numerator, denom2);

        sum += term;
    }

    return sum;
}
