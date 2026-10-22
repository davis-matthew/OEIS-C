/*
 * A327503  Number of steps to reach a fixed point starting with n and
 *          repeatedly taking the quotient by the maximum divisor that is 1 or
 *          not a perfect power (A327501, A327502).
 * https://oeis.org/A327503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps required to reach 1 by repeatedly *
 *            dividing n by the maximal divisor of n that is 1 or not a
 *            perfect power
 * Range:     u8 n <= 7171, u16 n <= 9241, u32 n <= 9241, u64 n <= 9241,
 *            u128 n <= 9241, big any n
 * Helpers:   factor_u64, u64_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/u64_gcd.h"

/* Euclidean gcd for 64‑bit integers. */

/* Return the number of steps for a given n. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t steps = 0;
    seq_t cur   = n;

    /* Temporary storage for factorization results. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    while (cur != (seq_t)1) {
        /* Factorise cur.  cur is assumed to fit in uint64_t. */
        factor_u64((uint64_t)cur, primes, exps, &cnt);

        /* Compute gcd of all exponents.  If it is 1, cur is not a perfect
           power and the next value becomes 1. */
        uint64_t g = exps[0];
        for (size_t i = 1; i < cnt; ++i)
            g = u64_gcd(g, exps[i]);

        if (g == 1) {
            /* Not a perfect power – one step reaches 1. */
            steps++;
            break;
        }

        if (cnt == 1) {
            /* Single prime power: next value is cur divided by that prime. */
            cur = cur / primes[0];
            steps++;
            continue;
        }

        /* Multi‑prime perfect power.  Find the smallest divisor
           factor that yields the maximal divisor not a perfect power. */
        uint64_t prefix[64], suffix[64];
        prefix[0] = exps[0];
        for (size_t i = 1; i < cnt; ++i)
            prefix[i] = u64_gcd(prefix[i-1], exps[i]);

        suffix[cnt-1] = exps[cnt-1];
        for (size_t i = cnt-2; ; --i) {
            suffix[i] = u64_gcd(suffix[i+1], exps[i]);
            if (i == 0) break;
        }

        seq_t best_factor = 0;   /* the smallest possible quotient */
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t g_other = 0;
            if (i == 0)
                g_other = suffix[1];
            else if (i == cnt-1)
                g_other = prefix[cnt-2];
            else
                g_other = u64_gcd(prefix[i-1], suffix[i+1]);

            uint64_t e = exps[i];
            uint64_t r_min = 0;  /* minimal decrement needed */

            /* Search for minimal r such that gcd(e - r, g_other) == 1. */
            for (uint64_t r = 1; r < e; ++r) {
                if (u64_gcd(e - r, g_other) == 1) {
                    r_min = r;
                    break;
                }
            }

            /* Compute factor = p[i]^r_min (the quotient). */
            seq_t factor = 1;
            for (uint64_t t = 0; t < r_min; ++t)
                factor *= primes[i];

            if (best_factor == 0 || factor < best_factor)
                best_factor = factor;
        }

        cur = best_factor;
        steps++;
    }

    return steps;
}
