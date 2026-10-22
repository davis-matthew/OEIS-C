/*
 * A348038  a(n) = A003968(n) / gcd(n, A003968(n)), where A003968 is
 *          multiplicative with a(p^e) = p*(p+1)^(e-1).
 * https://oeis.org/A348038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003968(n) / gcd(n, A003968(n)) where A003968 is
 *            multiplicative with A003968(p^e) = p * (p+1)^(e-1)
 * Range:     u8 n <= 63, u16 n <= 4095, u32 n <= 9382, u64 n <= 9382,
 *            u128 n <= 9382, big any n
 * Helpers:   _smallest_prime_factor, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n == 0) /* offset starts at 1, but protect against zero */
        return 0;

    uint64_t temp = (uint64_t)n;
    seq_t af = 1;                 /* A003968(n) */
    seq_t n_seq = (seq_t)n;        /* for gcd computation */

    while (temp > 1) {
        uint64_t p = _smallest_prime_factor(temp);
        if (p == 0) /* safety: treat remaining as prime */
            p = temp;

        size_t exp = 0;
        while ((temp % p) == 0) {
            temp /= p;
            exp++;
        }

        /* multiplicative contribution: p * (p+1)^(exp-1) */
        af *= (seq_t)p;              /* factor p once */
        for (size_t i = 1; i < exp; ++i)
            af *= ((seq_t)p + 1);    /* multiply (p+1) for remaining exponents */
    }

    seq_t g = gcd(af, n_seq);       /* gcd(n, A003968(n)) */
    return af / g;                  /* final a(n) */
}
