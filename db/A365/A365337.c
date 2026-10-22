/*
 * A365337  The sum of divisors of the largest exponentially odd number
 *          dividing n.
 * https://oeis.org/A365337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1796, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: multiplicative a(p^e) = (p^(e+1)-1)/(p-1) if e is odd, 
 *           (p^e-1)/(p-1)             if e is even.
 * a(n) = product over prime powers of n.
 */
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Computes x^e where e fits in idx_t */
static seq_t power_seq(seq_t x, idx_t e)
{
    seq_t result = 1;
    while (e) {
        if (e & 1UL) result *= x;
        x *= x;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Largest exponentially odd divisor: for each prime power, use exponent
       e if e is odd, else e-1.  Then compute sum of divisors (sigma). */
    /* Factor n */
    static uint64_t pr[64];
    static uint64_t ex[64];
    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        unsigned e = (unsigned)ex[i];
        unsigned f = (e & 1) ? e : e - 1;          /* exponent of largest odd divisor */
        seq_t num   = power_seq((seq_t)p, f + 1) - 1; /* p^(f+1)-1 */
        seq_t denom = (seq_t)p - 1;
        seq_t term  = num / denom;
        sigma *= term;
    }
    return sigma;
}
