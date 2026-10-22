/*
 * A351442  a(n) = A003958(sigma(n)), where A003958 is multiplicative with
 *          a(p^e) = (p-1)^e and sigma is the sum of divisors function.
 * https://oeis.org/A351442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A003958(sigma(n))
 * Range:     u8 n <= 143, u16 n <= 9435, u32 n <= 9435, u64 n <= 9435,
 *            u128 n <= 9435, big any n
 * Helpers:   pow_seq, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/pow_seq.h"

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
            result *= pow_seq(p - 1, (int)e);

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
