/*
 * A263010  Exceptional odd numbers D that do not admit a solution to the Pell
 *          equation X^2 - D Y^2 = +2.
 * https://oeis.org/A263010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A263010:  odd D ≡ 7 (mod 8) that are nonsquare,
 * have no prime factor ≡ 3 or 5 (mod 8), and admit no integer
 * solution to X² – D Y² = +2.  The terms are returned in increasing order.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

/* Return true iff the Pell equation X² – D Y² = 2 has an integer solution. */
static bool pell2_has_solution(uint64_t D)
{
    seq_t sq = floor_sqrt((seq_t)D);
    if (sq * sq == (seq_t)D)            /* D is a perfect square – skip */
        return false;

    uint64_t a0 = (uint64_t)sq;         /* floor(sqrt(D)) */
    uint64_t m = 0, d = 1, a = a0;
    seq_t p0 = 0, p1 = 1;
    seq_t q0 = 1, q1 = 0;

    for (uint64_t iter = 0; ; ++iter)
    {
        seq_t p = (seq_t)a * p1 + p0;
        seq_t q = (seq_t)a * q1 + q0;
        seq_t delta = p * p - (seq_t)D * q * q;
        if (delta == (seq_t)2)          /* solution found */
            return true;

        p0 = p1;  p1 = p;
        q0 = q1;  q1 = q;

        /* Advance continued‑fraction parameters */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        if (m == 0 && d == 1 && a == a0)   /* period completed */
            break;
        if (iter > 2 * a0 + 10)             /* safety guard */
            break;
    }
    return false;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* safety guard – offset is 1 */

    /* Cache of already generated terms */
    static seq_t *cache = NULL;
    static size_t cacheSize = 0, cacheCap = 0;

    /* Sieve state */
    static uint64_t *primes = NULL;
    static size_t primeCnt = 0;

    /* Current candidate odd number ≡ 7 (mod 8) */
    static uint64_t cand = 7;

    /* Generate terms until we have the n‑th one */
    while (cacheSize < (size_t)n)
    {
        /* Ensure we have all primes up to sqrt(cand) */
        seq_t s = floor_sqrt((seq_t)cand);
        uint64_t limit = (uint64_t)s;
        if (primes == NULL || (primeCnt > 0 && primes[primeCnt - 1] < limit))
        {
            uint64_t *new_primes = NULL;
            size_t new_cnt = 0;
            build_primes_upto(limit, &new_primes, &new_cnt);
            free(primes);
            primes = new_primes;
            primeCnt = new_cnt;
        }

        bool good = true;

        /* Skip perfect squares */
        seq_t sq = floor_sqrt((seq_t)cand);
        if (sq * sq == (seq_t)cand)
            good = false;

        /* Check prime‑factor condition: no prime ≡ 3 or 5 (mod 8). */
        if (good)
        {
            uint64_t temp = cand;
            for (size_t i = 0; i < primeCnt && primes[i] * primes[i] <= temp; ++i)
            {
                uint64_t p = primes[i];
                if (temp % p == 0)
                {
                    if (p % 8 == 3 || p % 8 == 5)
                    {
                        good = false;
                        break;
                    }
                    while (temp % p == 0)
                        temp /= p;
                }
            }
            if (good && temp > 1)           /* leftover large prime factor */
                if (temp % 8 == 3 || temp % 8 == 5)
                    good = false;
        }

        /* Verify that the Pell equation has no solution. */
        if (good && pell2_has_solution(cand))
            good = false;

        /* Store the term if it satisfies all conditions. */
        if (good)
        {
            if (cacheSize == cacheCap)
            {
                size_t newCap = cacheCap ? cacheCap * 2 : 64;
                cache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
                cacheCap = newCap;
            }
            cache[cacheSize++] = (seq_t)cand;
        }

        cand += 8;     /* next candidate ≡ 7 mod 8 */
    }

    return cache[n - 1];
}
