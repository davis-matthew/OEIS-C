/*
 * A304200  a(n) is the number of cyclic permutations with at most 2 ascents.
 * https://oeis.org/A304200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L(3,n) - n*L(2,n) when n ≠ 2 (mod 4) and n>2; a(n) =
 *            L(3,n) + L(3,n/2) - n*(L(2,n) + L(2,n/2)) when n == 2 (mod 4)
 *            and n>2; where L(k,n) = (1/n) * Σ_{d|n} μ(d) * k^{n/d} (Lyndon
 *            words). The following implementation computes L(k,n) via the
 *            divisor–sum formula and applies the above piecewise...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* --------------------------------------------------------------------- */
/* factor_uint64: factor n into primes (array size 16 is enough for n<=10000) */
static void factor_uint64(uint64_t n, uint64_t *primes, int *exps, size_t *cnt)
{
    *cnt = 0;
    if (n < 2) return;
    for (uint64_t p = 2; p * p <= n; p += (p == 2ULL ? 1ULL : 2ULL)) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[*cnt] = p;
            exps[*cnt] = e;
            (*cnt)++;
        }
    }
    if (n > 1ULL) {
        primes[*cnt] = n;
        exps[*cnt] = 1;
        (*cnt)++;
    }
}

/* --------------------------------------------------------------------- */
/* Recursive helper for L(k,n) using divisor sum with Möbius function */
static void dfs_Lk(uint64_t k, uint64_t n, size_t idx, uint64_t curr,
                   bool is_sf, int primecnt,
                   size_t pc, const uint64_t *pr, const int *exp,
                   seq_t *pos, seq_t *neg)
{
    if (idx == pc) {
        if (!is_sf) return;                 /* μ(d)=0 */
        int mu = (primecnt % 2 == 0) ? 1 : -1;
        seq_t term = big_pow((seq_t)k, (seq_t)(n / curr));
        if (mu > 0)
            *pos += term;
        else
            *neg += term;
        return;
    }
    uint64_t p = pr[idx];
    int emax = exp[idx];

    /* exponent e = 0 */
    dfs_Lk(k, n, idx + 1, curr, is_sf, primecnt,
           pc, pr, exp, pos, neg);

    /* exponent e >= 1 */
    uint64_t d = curr;
    for (int e = 1; e <= emax; ++e) {
        d *= p;
        bool new_is_sf = is_sf && (e == 1);
        int new_primecnt = primecnt + 1;
        dfs_Lk(k, n, idx + 1, d, new_is_sf, new_primecnt,
               pc, pr, exp, pos, neg);
    }
}

/* --------------------------------------------------------------------- */
/* L(k,n) via the Möbius divisor-sum formula */
static seq_t Lk(uint64_t k, uint64_t n)
{
    if (n == 0ULL) return (seq_t)1;                 /* convention */
    uint64_t primes[16];
    int exps[16];
    size_t pc;
    factor_uint64(n, primes, exps, &pc);

    seq_t pos = (seq_t)0;
    seq_t neg = (seq_t)0;

    dfs_Lk(k, n, 0, (uint64_t)1, true, 0, pc, primes, exps, &pos, &neg);

    /* difference should be non‑negative */
    seq_t diff = pos - neg;
    return diff / (seq_t)n;
}

/* --------------------------------------------------------------------- */
/* Main sequence function */
seq_t a(idx_t n)
{
    /* handle offset and trivial cases directly */
    if (n == 0 || n == 1 || n == 2)
        return (seq_t)1;

    seq_t L3 = Lk(3ULL, (uint64_t)n);
    seq_t L2 = Lk(2ULL, (uint64_t)n);

    if ((n & 3ULL) == 2ULL) {                     /* n ≡ 2 (mod 4) */
        uint64_t half = (uint64_t)(n / 2);
        seq_t L3h = Lk(3ULL, half);
        seq_t L2h = Lk(2ULL, half);
        seq_t mul = (seq_t)n * (L2 + L2h);
        return L3 + L3h - mul;
    } else {                                      /* other n > 2 */
        seq_t mul = (seq_t)n * L2;
        return L3 - mul;
    }
}
