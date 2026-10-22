/*
 * A355679  For any nonnegative number n with primorial base expansion Sum_{k
 *          >= 0} d_k * A002110(k), a(n) = Sum_{k >= 0} d_k * A002110(k) *
 *          (-1)^(Sum_{i < k} sign(d_i)).
 * https://oeis.org/A355679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Σ_k d_k · A002110(k) · (-1)^(Σ_{i<k} sign(d_i)), where
 *            the d_k are the digits of n in primorial base
 * Range:     i8 n <= 149, i16 n <= 2310, i32 n <= 2310, i64 n <= 2310,
 *            i128 n <= 2310, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Maximum number of primorials needed for 64‑bit integers is < 20. */
    const size_t MAX_DIGITS = 32;
    idx_t prim[MAX_DIGITS];
    size_t idx = 0;                /* highest primorial index with prim[idx] ≤ n */
    prim[0] = 1;                   /* P_0 = 1 (product of zero primes) */
    uint64_t prime = 2;            /* first prime */

    /* Build primorials up to n, guarding against overflow. */
    while (1) {
        if (prim[idx] > n / prime)            /* next primorial would exceed n */
            break;
        prim[idx + 1] = prim[idx] * prime;    /* P_{idx+1} = P_{idx} · prime */
        idx++;
        prime = next_prime(prime);            /* get next prime */
        if (idx + 1 >= MAX_DIGITS)           /* safety guard */
            break;
    }

    /* Compute the primorial base digits of n. */
    uint64_t digits[MAX_DIGITS];
    idx_t remaining = n;
    for (int j = (int)idx; j >= 0; --j) {
        digits[j] = remaining / prim[j];
        remaining %= prim[j];
    }

    /* Compute alternating sum according to the rule. */
    seq_t sum = 0;
    int sign = 1;   /* start with +1 */
    for (size_t j = 0; j <= idx; ++j) {
        uint64_t d = digits[j];
        if (d != 0) {
            sum += (seq_t)d * (seq_t)prim[j] * (seq_t)sign;
            sign = -sign;   /* toggle sign for next non‑zero digit */
        }
    }
    return sum;
}
