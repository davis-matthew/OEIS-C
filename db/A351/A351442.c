/*
 * A351442  a(n) = A003958(sigma(n)), where A003958 is multiplicative with
 *          a(p^e) = (p-1)^e and sigma is the sum of divisors function.
 * https://oeis.org/A351442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A003958(sigma(n))
 * Range:     u8 n <= 143, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 103 terms through n = 9435 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Computes A351442(n) = A003958(sigma(n)) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* out of OEIS offset, not used */

    /* Compute σ(n) */
    seq_t sigma = sigma_of(n);

    /* Factor σ(n) and accumulate the product (p-1)^e */
    seq_t rem = sigma;          /* remainder to factor */
    seq_t result = 1;           /* product (p-1)^e */

    /* Trial division over odd candidates, starting with 2 */
    for (seq_t p = 2; p <= rem / p; /* incrementation in body */)
    {
        seq_t e = 0;
        while (rem % p == 0) {
            rem /= p;
            e += 1;
        }
        if (e > 0)
            result *= ipow_i32(p - 1, (int)e);

        /* Move to the next candidate */
        if (p == 2)
            p = 3;
        else
            p += 2;
    }

    /* If a prime factor > sqrt(rem) remains, it appears once */
    if (rem > 1)
        result *= (rem - 1);

    return result;
}
