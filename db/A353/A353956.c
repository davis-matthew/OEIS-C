/*
 * A353956  Sum of the divisors of n whose arithmetic derivative is odd.
 * https://oeis.org/A353956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d' odd} d = Sum_{d|n, odd divisor, Ω(d) odd} d
 *            + Sum_{d|n, d even with exactly one 2} d This implementation
 *            enumerates all divisors of n and adds those whose arithmetic
 *            derivative is odd
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Recursive enumeration of divisors.
   pr, ex store the distinct prime factors and their exponents,
   pcnt is the number of primes.
   pos   : current prime index,
   cur   : product of chosen powers so far,
   k2    : exponent of 2 in the current divisor,
   sod   : sum of exponents of odd primes in the current divisor.
   res   : pointer to the running sum a(n). */
static void dfs(const uint64_t *pr, const uint64_t *ex, size_t pcnt,
                size_t pos, seq_t cur, uint64_t k2, uint64_t sod,
                seq_t *res)
{
    if (pos == pcnt) {
        if (k2 == 1 || (k2 == 0 && (sod & 1ULL)))
            *res += cur;
        return;
    }

    uint64_t p = pr[pos];
    seq_t curpow = 1;
    /* Iterate over all possible exponents for this prime. */
    for (uint64_t e = 0; e <= ex[pos]; ++e) {
        dfs(pr, ex, pcnt, pos + 1,
            cur * curpow,
            k2 + (p == 2 ? e : 0),
            sod + (p == 2 ? 0 : e),
            res);
        curpow *= (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    uint64_t nn = n;          /* cast to native unsigned integer for factoring */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;

    /* Factor out prime 2 explicitly. */
    if ((nn & 1ULL) == 0) {
        uint64_t e = 0;
        while ((nn & 1ULL) == 0) {
            nn >>= 1;
            ++e;
        }
        primes[pcnt] = 2;
        exps[pcnt]   = e;
        ++pcnt;
    }

    /* Trial division for odd primes. */
    for (uint64_t p = 3; p * p <= nn; p += 2) {
        if (nn % p == 0) {
            uint64_t e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }

    /* Any remaining factor is a prime > sqrt(original n). */
    if (nn > 1) {
        primes[pcnt] = nn;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    seq_t result = 0;
    dfs(primes, exps, pcnt, 0, (seq_t)1, 0, 0, &result);
    return result;
}
