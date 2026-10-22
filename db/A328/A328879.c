/*
 * A328879  If n = Product (p_j^k_j) then a(n) = Product (pi(p_j) + 1), where
 *          pi = A000720.
 * https://oeis.org/A328879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over distinct prime factors p of n of (pi(p)+1),
 *            where pi is the prime‑index function
 * Range:     u8 n <= 797, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function uses the provided compute_pi helper to obtain pi(p). */
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    idx_t m = n;
    seq_t ans = (seq_t)1;

    /* trial division to find distinct prime factors */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            /* p is a distinct prime factor */
            while (m % p == 0)
                m /= p;

            seq_t pi_val = compute_pi(p);          /* pi(p) */
            ans *= (pi_val + (seq_t)1);
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1) {
        seq_t pi_val = compute_pi(m);
        ans *= (pi_val + (seq_t)1);
    }

    return ans;
}
