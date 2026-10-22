/*
 * A348746  Fully multiplicative with a(2) = 3, a(3) = 5, a(A002144(n)) =
 *          A002144(1+n) and a(A002145(1+n)) = a(A002145(1+n)) for all n >= 1,
 *          where A002144 and A002145 give the primes of the form 4k+1 and
 *          4k+3 respectively.
 * https://oeis.org/A348746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(1) = 1. For n > 1 let n = ∏ p_i^{e_i} be its prime
 *            factorisation. Then a(n) = ∏ a(p_i)^{e_i}, where a(p) = 3 if p =
 *            2 a(p) = 5 if p = 3 a(p) = the next larger prime ≡ 1 (mod 4) if
 *            p ≡ 1 (mod 4) a(p) = p otherwise (p ≡ 3 (mod 4))
 * Range:     u8 n <= 39, u16 n <= 1199, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External helper headers.  The build system provides these. */
#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_prime_small.h"
#include "../utils/next_prime.h"

#include <stdint.h>

/* Map a prime p to its image a(p). */
static inline seq_t prime_image(uint64_t p)
{
    if (p == 2) return (seq_t)3;
    if (p == 3) return (seq_t)5;
    if ((p & 3ULL) == 1) {           /* p ≡ 1 (mod 4) */
        uint64_t q = next_prime(p + 1);
        while (q % 4ULL != 1ULL)     /* find next prime of same form */
            q = next_prime(q + 1);
        return (seq_t)q;
    }
    /* p ≡ 3 (mod 4) */
    return (seq_t)p;
}

/* Recursively compute a(n) via factorisation. */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;       /* a(1) = 1 */

    uint64_t nn = (uint64_t)n;          /* idx_t is uint64_t */
    uint64_t spf = _smallest_prime_factor(nn); /* smallest prime factor */

    if (spf == nn)                     /* n is prime */
        return prime_image(spf);

    /* n is composite: factor out spf^e and recurse on the remaining part. */
    uint64_t exp = 0;
    while (nn % spf == 0) {
        nn /= spf;
        ++exp;
    }

    seq_t base = prime_image(spf);
    seq_t res  = 1;
    for (uint64_t k = 0; k < exp; ++k)
        res *= base;

    return res * a((idx_t)nn);
}
