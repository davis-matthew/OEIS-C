/*
 * A328620  Number of nonleading zeros in primorial base expansion of n, a(0)
 *          = 0 by convention.
 * https://oeis.org/A328620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of nonleading zeros in the primorial base *
 *            expansion of n. That is, write n = Σ d_i * P_i where * P_i =
 *            product of the first i primes and 0 ≤ d_i < next_prime(i). *
 *            a(n) counts how many digits d_0 … d_{k-1} are zero, where k is
 *            the * highest index with a non‑zero digit. a(0) = 0 by...
 * Range:     u8 n <= 7121, u16 n <= 7121, u32 n <= 7121, u64 n <= 7121,
 *            u128 n <= 7121, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------  dynamic tables for primes and their primorial weights  ---------- */
static uint64_t *primes = NULL;          /* primes[0 .. prime_cnt-1]          */
static uint64_t *weights = NULL;        /* weights[0 .. prime_cnt]  (weights[0]=1) */
static size_t prime_cnt = 0;            /* number of primes stored          */
static size_t prim_cap = 0;             /* allocated capacity for primes     */

/* enlarge the arrays to hold at least new_cap primes (weights new_cap+1)    */
static void grow_arrays(size_t new_cap)
{
    uint64_t *np = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!np) exit(EXIT_FAILURE);
    primes = np;

    uint64_t *nw = (uint64_t *)realloc(weights, (new_cap + 1) * sizeof(uint64_t));
    if (!nw) exit(EXIT_FAILURE);
    weights = nw;

    prim_cap = new_cap;
}

/* add primes until the next primorial weight exceeds n.               */
static void ensure_primes_upto(uint64_t n)
{
    if (!primes) {                /* first call: allocate small tables   */
        grow_arrays(16);
        weights[0] = 1;           /* weight for 0 primes = 1             */
    }

    /* generate primes while the current primorial <= n                */
    while (weights[prime_cnt] <= n) {
        uint64_t candidate = (prime_cnt == 0 ? 2 : primes[prime_cnt - 1] + 1);
        if (candidate > 2 && (candidate & 1) == 0) candidate++; /* skip even */

        /* primality test using already known primes                   */
        while (true) {
            bool isprime = true;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = primes[i];
                if (p * p > candidate) break;
                if (candidate % p == 0) { isprime = false; break; }
            }
            if (isprime) break;
            candidate += (candidate == 2 ? 1 : 2);  /* next odd number    */
        }

        /* store new prime and its primorial weight                    */
        if (prime_cnt == prim_cap) grow_arrays(prim_cap * 2);
        primes[prime_cnt] = candidate;
        weights[prime_cnt + 1] = weights[prime_cnt] * candidate;
        ++prime_cnt;
    }
}

/* ----------  main sequence function  -------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* ensure primorials > n are known                                    */
    ensure_primes_upto((uint64_t)n);

    /* find the highest index k with weights[k] <= n                      */
    size_t k = 0;
    while (k + 1 < prime_cnt + 1 && weights[k + 1] <= (uint64_t)n) ++k;

    /* extract digits of the primorial representation                   */
    unsigned int digits[64];          /* plenty for 0 ≤ n ≤ 10 000          */
    uint64_t rem = n;
    for (long long i = k; ; --i) {
        uint64_t w = weights[(size_t)i];
        digits[(size_t)i] = (unsigned int)(rem / w);
        rem %= w;
        if (i == 0) break;
    }

    /* locate most significant non‑zero digit                            */
    size_t msd = k;
    while (msd > 0 && digits[msd] == 0) --msd;

    /* count zeros below that digit                                      */
    seq_t zeros = 0;
    for (size_t i = 0; i < msd; ++i)
        if (digits[i] == 0) ++zeros;

    return zeros;
}
