/*
 * A302792  a(1) = 1; for n>1, a(n) = n/(smallest Fermi-Dirac factor of n).
 * https://oeis.org/A302792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / smallest_Fermi-Dirac_factor(n) Smallest Fermi-Dirac
 *            factor of n is the smallest divisor > 1 d of n such that n/d is
 *            an infinitary divisor of n
 * Range:     u8 n <= 510, u16 n <= 8838, u32 n <= 8838, u64 n <= 8838,
 *            u128 n <= 8838, big any n
 * Helpers:   pow_seq_p_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_seq_p_k.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;      /* a(1) = 1 */

    uint64_t N = (uint64_t)n;
    uint64_t temp = N;
    seq_t best_factor = 0;    /* 0 means “not set yet” */

    /* Factorization by trial division (fast enough for n up to ~10^6). */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }

            /* Find the minimal k (1 <= k <= exp) such that
               (exp - k) is a submask of exp. */
            int chosen_k = exp;  /* default if no smaller k works */
            for (int k = 1; k <= exp; ++k) {
                if (((exp - k) & ~exp) == 0) {
                    chosen_k = k;
                    break;
                }
            }

            seq_t candidate = pow_seq_p_k(p, chosen_k);
            if (best_factor == 0 || candidate < best_factor)
                best_factor = candidate;
        }
    }

    /* Remaining prime factor (if > 1). */
    if (temp > 1) {
        /* temp is prime with exponent 1. */
        seq_t candidate = (seq_t)temp;
        if (best_factor == 0 || candidate < best_factor)
            best_factor = candidate;
    }

    /* best_factor should now be > 1 */
    return (seq_t)n / best_factor;
}
