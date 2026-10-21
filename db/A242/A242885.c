/*
 * A242885  Least number k such that (k^k+n^n)/(k+n) is an integer.
 * https://oeis.org/A242885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least k such that (k^k + n^n)/(k + n) is an integer *
 *            (OEIS A242885). If n is odd, a(n)=1. For even n we search k=1..n
 * Range:     u8 n <= 507, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"   /* declares seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

seq_t a(idx_t n)
{
    /* Odd n always gives k = 1 */
    if (n & 1) return (seq_t)1;

    /* For even n try k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t m = (seq_t)k + (seq_t)n;          /* modulus m = k + n */
        seq_t ak = mod_pow((seq_t)k, k, m);     /* k^k mod m */
        seq_t an = mod_pow((seq_t)n, n, m);     /* n^n mod m */
        if (((ak + an) % m) == (seq_t)0)        /* (k^k + n^n) divisible by m */
            return (seq_t)k;
    }
    return (seq_t)n;   /* guarantee: a(n) <= n, so this line is never reached */
}
