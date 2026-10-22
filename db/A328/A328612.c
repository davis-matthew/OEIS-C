/*
 * A328612  Starting with m = 1, p = 2 and x = n, iterate m -> m *
 *          p^valuation(x,p), x -> floor(x/p), p -> {next prime after p},
 *          until x is zero. Then a(n) = {the value of m at the end of
 *          iteration}.
 * https://oeis.org/A328612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 8191, u32 n <= 9655, u64 n <= 9655,
 *            u128 n <= 9655, big any n
 * Helpers:   is_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328612:  a(n) is computed by iterating over successive primes.
 * Start with m=1, p=2, x=n.
 * While x > 0:
 *   v = valuation of x at prime p (exponent of p dividing x),
 *   m *= p^v,
 *   x = floor(x / p),
 *   p = next prime after the current p.
 * Return the final value of m. */
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)1;
    idx_t x = n;
    idx_t p = 2;

    while (x > 0) {
        /* compute valuation of x at prime p */
        idx_t temp = x;
        idx_t v = 0;
        while (temp % p == 0) {
            temp /= p;
            ++v;
        }

        /* multiply m by p^v */
        for (idx_t i = 0; i < v; ++i)
            m = seq_mul(m, (seq_t)p);

        /* divide x by p */
        x /= p;

        /* find next prime after p */
        idx_t cand = p + 1;
        while (!is_prime(cand)) ++cand;
        p = cand;
    }

    return m;
}
