/*
 * A350339  Sum of the nontrivial divisors of n that are the product of up to
 *          3 (not necessarily distinct) primes.
 * https://oeis.org/A350339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, changed
 * Formula:   a(n) = Sum_{d | n , 1 <= Omega(d) <= 3} d
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where Omega(d) is the total number of prime factors of d counting multiplicity,
    and the sum is over all non‑trivial divisors d of n.
    Implemented by factoring n with trial division, enumerating all divisors
    whose total exponent sum is 1, 2 or 3, and accumulating their values. */

#include <stddef.h>
#include <stdint.h>

/* Maximum number of distinct prime factors expected for the given n.
   For n <= 10^8 this is safely below 10. */
#define MAX_FACTORS 10

static void factor_uint64(uint64_t n, uint64_t *factors, uint32_t *exps,
                          size_t *cnt)
{
    *cnt = 0;
    if (n < 2) return;

    /* handle factor 2 */
    if (n % 2 == 0) {
        factors[(*cnt)] = 2;
        uint32_t e = 0;
        while ((n % 2) == 0) {
            n /= 2;
            e++;
        }
        exps[(*cnt)++] = e;
    }
    /* odd factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            factors[(*cnt)] = p;
            uint32_t e = 0;
            while ((n % p) == 0) {
                n /= p;
                e++;
            }
            exps[(*cnt)++] = e;
            if (*cnt >= MAX_FACTORS) break; /* safety */
        }
    }
    if (n > 1 && *cnt < MAX_FACTORS) {
        factors[(*cnt)] = n;
        exps[(*cnt)++] = 1;
    }
}

/* Recursive enumeration of divisors up to Omega <= 3.
   curr is the current divisor value (converted to seq_t in the caller). */
static void dfs_divisors(size_t idx, seq_t curr, uint32_t omega,
                         const uint64_t *pf, const uint32_t *pe,
                         size_t pf_cnt, seq_t *sum)
{
    if (idx == pf_cnt) {
        if (omega > 0 && omega <= 3) {
            *sum += curr;
        }
        return;
    }
    seq_t base = 1;
    for (uint32_t exp = 0; exp <= pe[idx]; ++exp) {
        dfs_divisors(idx + 1, curr * base, omega + exp,
                     pf, pe, pf_cnt, sum);
        base *= (seq_t)pf[idx];
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    uint64_t N = (uint64_t)n;
    uint64_t factors[MAX_FACTORS];
    uint32_t exps[MAX_FACTORS];
    size_t cnt;

    factor_uint64(N, factors, exps, &cnt);

    seq_t sum = 0;
    dfs_divisors(0, 1, 0, factors, exps, cnt, &sum);
    return sum;
}
