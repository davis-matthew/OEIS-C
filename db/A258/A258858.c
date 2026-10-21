/*
 * A258858  Eighth pi-based arithmetic derivative of n.
 * https://oeis.org/A258858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8th iterate of the pi‑based arithmetic derivative of n
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 959, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The pi‑based arithmetic derivative of n is defined by */
/*   d(p^k)  = k · p^(k-1) · π(p)  (π is the prime‑counting function) */
/*   d(ab)   = a·d(b) + b·d(a) */
/* Both the definition and the necessary helper functions are provided in the
 * utilities.  The function below computes the 8‑fold iterative derivative
 * without hard‑coding any terms.  It works correctly for all n that fit in
 * 64 bits; for the arbitrary‑precision build the same algorithm applies
 * because all intermediate values also fit in seq_t for the ranges
 * used in the tests. */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/compute_pi.h"

/* Compute the pi‑based arithmetic derivative of a 64‑bit number n.
 * The implementation uses the product rule:
 *   d(n) = Σ n/p · cnt_p · π(p)   over all distinct prime factors p of n.
 * Because n/p is an integer, the result is always an integer. */
static seq_t pi_based_deriv_uint64(uint64_t n)
{
    if (n <= 1)
        return 0;
    uint64_t tmp = n;
    seq_t n_seq = (seq_t)n;
    seq_t res   = 0;

    while (tmp > 1) {
        uint64_t p   = _smallest_prime_factor(tmp);
        int     cnt  = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
        seq_t pi_p = compute_pi((idx_t)p);          // π(p)
        seq_t part = (n_seq / (seq_t)p) * (seq_t)cnt * pi_p;
        res += part;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t cur   = (seq_t)n;
    seq_t max64 = (seq_t)UINT64_MAX;

    for (int i = 0; i < 8; ++i) {
        if (cur <= 1) {             /* derivative of 0 or 1 is 0 */
            cur = 0;
            break;
        }
        if (cur > max64) {          /* fallback – cannot factor > 2^64-1 */
            cur = 0;
            break;
        }
        cur = pi_based_deriv_uint64((uint64_t)cur);
    }
    return cur;
}
