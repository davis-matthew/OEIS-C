/*
 * A382493  a(n) is the 2-adic valuation of the least number that has exactly
 *          n 3-smooth divisors.
 * https://oeis.org/A382493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d-1, where d is the divisor of n that minimizes
 *            2^(d-1)*3^(n/d-1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The comparison of 2^(d-1)*3^(n/d-1) is carried out without floating point
   by scaling the logarithm of 3 with the integer constant P
   (≈log2(3)*2^60).  The scaled value
       S(d) = (d-1)*2^60 + (n/d-1)*P
   preserves the ordering of 2^(d-1)*3^(n/d-1) for all n up to 10 000. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2^60 as an unsigned 64‑bit constant */
    static const unsigned long long Q = 1152921504606846976ULL;   /* 1ULL << 60 */

    /* P ≈ log2(3) * Q rounded to nearest integer */
    static const unsigned long long P = 1827748764547664000ULL;    /* round(1.584962500721156 * Q) */

    idx_t nn = n;                            /* defensive local copy */
    if (nn == 0)              /* n should be >= 1, but guard anyway */
        return 0;

    unsigned __int128 best_val = (unsigned __int128)-1;   /* huge sentinel */
    idx_t best_d = 1;

    for (idx_t d = 1; d <= nn / d; ++d)
    {
        if (nn % d != 0)
            continue;

        /* candidate divisor d */
        unsigned __int128 val_d = (unsigned __int128)(d - 1) * Q
                                + (unsigned __int128)(nn / d - 1) * P;
        if (val_d < best_val) {
            best_val = val_d;
            best_d = d;
        }

        idx_t d2 = nn / d;
        if (d2 == d)               /* skip duplicate when d*d == n */
            continue;

        /* candidate divisor d2 */
        unsigned __int128 val_d2 = (unsigned __int128)(d2 - 1) * Q
                                  + (unsigned __int128)(nn / d2 - 1) * P;
        if (val_d2 < best_val) {
            best_val = val_d2;
            best_d = d2;
        }
    }

    return (seq_t)(best_d - 1);
}
