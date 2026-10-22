/*
 * A392539  a(n) = the least common multiple of all natural numbers k such
 *          that phi(k) <= n.
 * https://oeis.org/A392539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product over primes p of p^{e_p(n)}, where e_p(n) = max
 *            e>=1 such that p^{e-1}*(p-1) <= n. Thus a(n) = lcm of all k with
 *            phi(k) <= n. This follows from the multiplicativity of phi and
 *            the fact that p^e alone gives the maximal exponent of p among
 *            numbers with phi(k) <= n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

static uint64_t *primes = NULL;          /* array of primes up to current limit */
static size_t primes_count = 0;           /* number of primes stored   */
static idx_t primes_limit = 0;           /* highest n+1 for which primes were generated */

/* Helper to regenerate primes up to a new limit */
static void regenerate_primes(idx_t limit)
{
    if (primes != NULL) {
        free(primes);
        primes = NULL;
        primes_count = 0;
        primes_limit = 0;
    }
    /* build_primes_upto returns primes <= limit */
    build_primes_upto(limit, &primes, &primes_count);
    primes_limit = limit;
}

seq_t a(idx_t n)
{
    /* ensure primes up to n+1 are available */
    if (n + 1 > primes_limit) {
        regen:
        regenerate_primes((idx_t)(n + 1));
    }

    seq_t result = (seq_t)1;    /* initial value */

    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        if (p > n + 1) break;   /* primes are sorted ascending */

        /* maximum allowed value for p^{e-1} */
        idx_t maxPow = n / (p - 1);

        /* find largest e such that p^{e-1} <= maxPow */
        idx_t e = 1;
        idx_t curr = 1; /* p^{e-1} */
        while (curr * p <= maxPow) {
            curr *= p;
            ++e;
        }

        /* multiply result by p^e (repeatedly due to lack of direct pow for seq_t) */
        seq_t pseq = (seq_t)p;
        for (idx_t j = 0; j < e; ++j) {
            result = seq_mul(result, pseq);
        }
    }

    return result;
}
