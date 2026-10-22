/*
 * A377533  Expansion of e.g.f. 1/(1 - x * exp(x^2))^2.
 * https://oeis.org/A377533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = n! * Σ_{k=0..⌊n/2⌋} (n-2k+1) * (n-2k)^k / k!
 * Equivalently: a(n) = Σ_{k=0..⌊n/2⌋} (n! / k!) * (n-2k+1) * (n-2k)^k
 */

#include <stdint.h>
#include "../utils/factorial.h"   /* Provides seq_t factorial(idx_t n) */

/* Integer exponentiation for seq_t (base ^ exp) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != (idx_t)0) {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    seq_t fact_n = factorial(n);     /* n!  */
    seq_t c = fact_n;                /* will hold n! / k!  */
    seq_t sum = (seq_t)0;
    idx_t max_k = n / (idx_t)2;

    for (idx_t k = (idx_t)0; k <= max_k; ++k) {
        idx_t m = n - (idx_t)2 * k;       /* m = n - 2k  */
        seq_t term = c * (seq_t)(m + (idx_t)1);   /* (n! / k!) * (m+1) */
        term *= pow_seq((seq_t)m, k);          /* multiply by m^k  */
        sum += term;                           /* accumulate  */

        /* Update c for next k: (n! / (k+1)!) = (n! / k!) / (k+1)  */
        if (k < max_k)
            c /= (seq_t)(k + (idx_t)1);
    }
    return sum;
}
