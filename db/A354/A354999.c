/*
 * A354999  a(n) = n divided by the smallest divisor d of n for which
 *          A344005(d) = A344005(n), where A344005(n) is the smallest positive
 *          integer m such that n divides m*(m+1).
 * https://oeis.org/A354999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n divided by the smallest divisor d of n for which
 *            A344005(d) = A344005(n), where A344005(k) is the smallest
 *            positive m such that k divides m*(m+1)
 * Range:     u8 n <= 1710, u16 n <= 8905, u32 n <= 8905, u64 n <= 8905,
 *            u128 n <= 8905, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- helpers for small factorisation ---------- */

/* factorise m (m <= 10000) into prime powers.
   primes[i], exps[i] hold the i-th prime and its exponent.
   Returns number of distinct prime factors. */
static size_t factorise(uint64_t m, uint64_t primes[], uint64_t exps[])
{
    size_t cnt = 0;
    if (m <= 1)
        return 0;
    /* factor 2 */
    if (m % 2 == 0) {
        primes[cnt] = 2;
        uint64_t e = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++e;
        }
        exps[cnt] = e;
        ++cnt;
    }
    /* odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            primes[cnt] = p;
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {
        primes[cnt] = m;
        exps[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* ---------- A344005 for small n (<= 10000) ---------- */

/* smallest positive m such that n divides m*(m+1) */
static uint64_t A344005(uint64_t n)
{
    if (n == 1)
        return 1;
    for (uint64_t m = 1; ; ++m) {
        uint64_t prod = m * (m + 1);
        if (prod % n == 0)
            return m;
    }
}

/* ---------- recursive divisor search ----------
   current divisor = prod * p_i^e_i for processed primes
   At leaf, evaluate A344005 on this divisor. Keep smallest divisor
   satisfying the equality with target. ---------- */
static void find_smallest_divisor(const uint64_t primes[],
                                  const uint64_t exps[],
                                  size_t idx,
                                  size_t nfac,
                                  uint64_t prod,
                                  uint64_t target,
                                  uint64_t n,
                                  uint64_t *best_d)
{
    if (idx == nfac) {
        /* prod is a divisor of n */
        if (prod < *best_d && A344005(prod) == target)
            *best_d = prod;
        return;
    }
    /* Pre‑compute powers of this prime */
    uint64_t p = primes[idx];
    uint64_t p_pow = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        uint64_t new_prod = prod * p_pow;
        /* prune if already not smaller than current best */
        if (new_prod < *best_d)
            find_smallest_divisor(primes, exps, idx + 1, nfac,
                                  new_prod, target, n, best_d);
        p_pow *= p;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for index < 1, but return 0 for safety */
        return (seq_t)0;

    /* factorise n */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t nfac = factorise((uint64_t)n, primes, exps);

    /* compute A344005(n) */
    uint64_t target = A344005((uint64_t)n);

    /* search for smallest divisor d with same A344005 value */
    uint64_t best_d = (uint64_t)n;  /* n itself is always a divisor */
    find_smallest_divisor(primes, exps, 0, nfac,
                          1ULL, target, (uint64_t)n, &best_d);

    /* result: n / best_d */
    return ((seq_t)n) / ((seq_t)best_d);
}
