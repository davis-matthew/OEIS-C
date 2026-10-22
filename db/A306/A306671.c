/*
 * A306671  a(n) = gcd(tau(n), pod(n)) where tau(k) = the number of the
 *          divisors of k (A000005) and pod(k) = the product of the divisors
 *          of k (A007955).
 * https://oeis.org/A306671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(tau(n), pod(n)) where pod(n) = n^{tau(n)/2}
 * Range:     u8 n <= 9759, u16 n <= 9759, u32 n <= 9759, u64 n <= 9759,
 *            u128 n <= 9759, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper to factor a uint64_t number.
   Provided by the utilities. */
#include "../utils/factor_u64.h"

/* Computes a(n) = gcd(tau(n), pod(n)) for n >= 1. */
seq_t a(idx_t n)
{
    /* Special case: n == 0 is not used, but guard anyway. */
    if (n == 0)
        return 0;

    /* Factor n. 64-bit numbers have at most 65 distinct prime factors. */
    uint64_t pn[65], en[65];
    size_t cnt_n;
    factor_u64((uint64_t)n, pn, en, &cnt_n);

    /* Compute tau(n) = ∏ (ei + 1) as a seq_t. */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt_n; ++i)
        tau = tau * (en[i] + 1);

    /* Convert tau to uint64_t for further use.  Tau is small for 64‑bit n. */
    uint64_t d = (uint64_t)tau;  /* this is d = tau(n) */

    /* Factor d (tau(n)). */
    uint64_t pd[65], ed[65];
    size_t cnt_d;
    factor_u64(d, pd, ed, &cnt_d);

    /* Compute the GCD.  For each prime q | d, determine the exponent in
       the product of divisors pod(n) = n^{d/2}.

       exponent_pod(q) = e_i * d / 2   (if q divides n),
                        0                otherwise.
       exponent_d(q)   = ed[j]
       exponent_gcd(q) = min(exponent_d, exponent_pod). */
    seq_t result = 1;

    for (size_t j = 0; j < cnt_d; ++j) {
        uint64_t q = pd[j];
        size_t exp_d = ed[j];

        /* Find exponent of q in n (if any). */
        size_t exp_n = 0;
        for (size_t i = 0; i < cnt_n; ++i) {
            if (pn[i] == q) {
                exp_n = en[i];
                break;
            }
        }

        if (exp_n == 0 || exp_d == 0)
            continue; /* q does not divide n, or exponent in d is zero */

        /* exponent in pod = exp_n * d / 2.  Use 128‑bit to avoid overflow. */
        unsigned __int128 tmp = (unsigned __int128)exp_n * d;
        size_t exp_pod = (size_t)(tmp / 2);

        size_t exp_gcd = exp_d < exp_pod ? exp_d : exp_pod;
        if (exp_gcd == 0)
            continue;

        /* Multiply result by q^{exp_gcd}.  exp_gcd is small. */
        seq_t q_seq = (seq_t)q;
        for (size_t k = 0; k < exp_gcd; ++k)
            result *= q_seq;
    }

    return result;
}
