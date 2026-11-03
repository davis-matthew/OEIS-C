/*
 * A341578  a(n) is the minimum number of total votes needed for one party to
 *          win if there are n^2 voters divided into equal districts.
 * https://oeis.org/A341578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min over divisors d of n^2 of * (⌊d/2⌋ + 1) * (⌊n^2/(2d)⌋
 *            + 1), and if n is even also * consider (n/2 + 1)^2 – 1. *
 *            Parameters: idx_t n – 1‑based sequence index (>=1) * Returns:
 *            seq_t – minimal number of votes * * The implementation uses a
 *            recursive divisor generator * to avoid any dynamic...
 * Range:     u8 n <= 15, u16 n <= 306, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 76 terms through n = 4906 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Recursive divisor enumeration.
 *  pi      : current prime index
 *  cur     : current divisor value
 *  primes  : array of prime factors of n
 *  maxexp  : array of (2*exponent_in_n) for each prime
 *  pcnt    : number of distinct primes
 *  n2      : n^2 (seq_t)
 *  best    : pointer to current best value
 */
static void
divisor_dfs(const uint64_t primes[], const int maxexp[],
            int pi, int pcnt, seq_t cur, seq_t n2, seq_t *best)
{
    if (pi == pcnt) {                       /* leaf: evaluate divisor cur */
        seq_t f1   = cur / 2 + 1;            /* floor(cur/2)+1 */
        seq_t denom = cur * 2;               /* 2*cur */
        seq_t f2   = n2 / denom + 1;         /* floor(n^2/(2*cur))+1 */
        seq_t cand = f1 * f2;                /* candidate value */
        if (*best == 0 || cand < *best)
            *best = cand;
        return;
    }

    /* iterate exponent for current prime: 0 .. maxexp[pi] */
    seq_t p_pow = 1;
    for (int e = 0; e <= maxexp[pi]; ++e) {
        divisor_dfs(primes, maxexp, pi + 1, pcnt,
                    cur * p_pow, n2, best);
        /* next exponent */
        p_pow *= (seq_t)primes[pi];
    }
}

seq_t a(idx_t n)
{
    /* Handle trivially */
    if (n == 0) return 0;

    /* n^2 as seq_t */
    seq_t n2 = (seq_t)n * (seq_t)n;

    /* factorise n into primes and exponents */
    uint64_t primes[64];
    int      exps[64];          /* exponent of each prime in n */
    int      pcnt = 0;
    uint64_t tmp  = (uint64_t)n;

    if (tmp % 2 == 0) {
        int c = 0;
        while (tmp % 2 == 0) { tmp /= 2; ++c; }
        primes[pcnt] = 2;
        exps[pcnt]   = c;
        ++pcnt;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int c = 0;
            while (tmp % p == 0) { tmp /= p; ++c; }
            primes[pcnt] = p;
            exps[pcnt]   = c;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* precompute 2*exponent for each prime (since we need d | n^2) */
    int maxexp[64];
    for (int i = 0; i < pcnt; ++i)
        maxexp[i] = 2 * exps[i];

    seq_t best = 0;        /* 0 indicates uninitialised */

    /* enumerate all divisors of n^2 */
    divisor_dfs(primes, maxexp, 0, pcnt, (seq_t)1, n2, &best);

    /* for even n also consider ((n/2)+1)^2 - 1 */
    if ((n & 1) == 0) {
        seq_t half = (seq_t)(n / 2);
        seq_t val_even = (half + 1) * (half + 1) - 1;
        if (val_even < best)
            best = val_even;
    }

    return best;
}
