/*
 * A323495  Irregular table read by rows: T(n,k) = (2*k+1)^(-1) mod 2^n, 0 <=
 *          k <= 2^(n-1) - 1.
 * https://oeis.org/A323495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = (2*k+1)^(2^(r-1)-1) mod 2^r, where r = ceil(log2(n+1))
 *            and k = n - 2^(r-1). This is the modular inverse of the odd
 *            number 2*k+1 modulo 2^r
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for the regular OEIS offset 1. */
seq_t a(idx_t n)
{
    /* Find the row index r such that 2^(r-1) <= n <= 2^r-1. */
    unsigned long long nn = (unsigned long long)n;      /* n >= 1 */
    unsigned long long tmp = 1;
    unsigned int r = 0;
    while (tmp <= nn) {
        tmp <<= 1;
        ++r;
    }
    /* Compute row start index s = 2^(r-1). */
    unsigned long long s = 1ULL << (r - 1);
    /* Position of n within its row: k = 0 .. 2^(r-1)-1. */
    unsigned long long k = nn - s;
    /* Base of the inverse: 2*k + 1 (always odd). */
    unsigned long long base = 2ULL * k + 1ULL;

    /* Modulus: 2^r.  For r <= 63, 1ULL<<r is defined. */
    unsigned long long mod_mask = (1ULL << r) - 1ULL;  /* mask for modulo 2^r */
    unsigned long long exp = ((1ULL << (r - 1)) - 1ULL);   /* exponent 2^(r-1)-1 */

    /* Modular exponentiation via binary exponentiation.  All
       intermediate products are masked to r bits. */
    unsigned long long result = 1ULL;
    unsigned long long b = base & mod_mask;
    while (exp) {
        if (exp & 1ULL) {
            result = (result * b) & mod_mask;
        }
        b = (b * b) & mod_mask;
        exp >>= 1ULL;
    }

    return (seq_t)result;
}
