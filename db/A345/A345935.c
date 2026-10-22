/*
 * A345935  Number of divisors d of n for which A002034(d) = A002034(n), where
 *          A002034(n) is the smallest positive integer k such that n divides
 *          k!.
 * https://oeis.org/A345935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n with A002034(d) = A002034(n)
 * Range:     u8 n <= 9840, u16 n <= 9840, u32 n <= 9840, u64 n <= 9840,
 *            u128 n <= 9840, big any n
 * Helpers:   factorise_u64, min_k_prime_power, smarandache_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                A002034(n) is the smallest k such that n | k!          */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factorise_u64.h"
#include "../utils/min_k_prime_power.h"
#include "../utils/smarandache_u64.h"

/* ------------------------------------------------------------ */
/* Factorisation of a 64‑bit number (trial division).          */
/* primes  : array where prime factors are stored                 */
/* exps    : array where corresponding exponents are stored       */
/* *cnt    : number of distinct prime factors                     */

/* ------------------------------------------------------------ */
/* For a prime power p^e, return the minimal k such that p^e | k! */

/* ------------------------------------------------------------ */
/* Kempner (Smarandache) function for a 64‑bit integer.        */

/* ------------------------------------------------------------ */
/* Recursively enumerate all divisors of n using its factorisation */
/* ------------------------------------------------------------ */
static void rec_divisors(size_t idx, size_t total,
                         const uint64_t *pr, const int *ex,
                         int *curExp,
                         seq_t Smn,
                         seq_t *cnt)
{
    if (idx == total)
    {
        /* Compute smarandache value for this divisor. */
        seq_t Sd = 0;
        bool hasPrime = false;
        for (size_t j = 0; j < total; ++j)
        {
            if (curExp[j] > 0)
            {
                hasPrime = true;
                seq_t t = min_k_prime_power(pr[j], curExp[j]);
                if (t > Sd) Sd = t;
            }
        }
        if (!hasPrime) Sd = 1;          /* divisor = 1 */
        if (Sd == Smn) (*cnt) += (seq_t)1;
        return;
    }

    for (int e = 0; e <= ex[idx]; ++e)
    {
        curExp[idx] = e;
        rec_divisors(idx + 1, total, pr, ex, curExp, Smn, cnt);
    }
}

/* ------------------------------------------------------------ */
/* Main entry point for sequence A345935.                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    seq_t Smn = smarandache_u64(N);

    uint64_t primes[64];
    int exps[64];
    size_t cnt;
    factorise_u64(N, primes, exps, &cnt);

    int curExp[64] = {0};
    seq_t result = (seq_t)0;

    rec_divisors(0, cnt, primes, exps, curExp, Smn, &result);
    return result;
}
