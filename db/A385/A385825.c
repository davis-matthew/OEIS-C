/*
 * A385825  a(n) = Sum_{k=0..n} (binomial(n, k) mod 5).
 * https://oeis.org/A385825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (binomial(n,k) mod 5)
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9669 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom_mod5(idx_t n, idx_t k)
{
    if (k > n) return 0;
    seq_t res = 1;
    /* binomial coefficients for digits 0..4 (exact values, not modulo) */
    static const unsigned char tbl[5][5] = {
        {1,0,0,0,0},
        {1,1,0,0,0},
        {1,2,1,0,0},
        {1,3,3,1,0},
        {1,4,6,4,1}
    };
    while (n > 0 || k > 0) {
        unsigned char nd = n % 5;
        unsigned char kd = k % 5;
        if (kd > nd) return 0;
        /* multiply the tiny binomial value and reduce modulo 5 */
        res = (seq_t)((seq_t)res * (seq_t)tbl[nd][kd] % 5);
        n /= 5;
        k /= 5;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        sum += binom_mod5(n, k);
    }
    return sum;
}
