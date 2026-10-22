/*
 * A381733  Number of divisors d of n such that 2^omega(n + d) = tau(n + d),
 *          where omega = A001221 and tau = A000005.
 * https://oeis.org/A381733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { d | n : n + d is squarefree }
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   build_primes_upto, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence generator for OEIS A381733
   Using the observation that 2^omega(m) = tau(m)
   iff m is squarefree.  Thus we count divisors d of n
   for which n + d is squarefree. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_squarefree.h"

static uint64_t *PR = NULL;   /* prime list */
static size_t   PR_CNT = 0;   /* number of primes */

static void init_primes(void)
{
    if (!PR) {
        size_t cnt;
        /* 200000 is comfortably larger than sqrt(2n) for all n we handle */
        build_primes_upto(200000, &PR, &cnt);
        PR_CNT = cnt;
    }
}

/* Recursively enumerate all divisors of n, and count those
   for which n + d is squarefree. */
static void dfs_divisors(size_t idx, uint64_t curr,
                         const uint64_t *fac, const unsigned int *exp,
                         size_t fac_cnt, idx_t n, seq_t *cnt)
{
    if (idx == fac_cnt) {
        uint64_t d = curr;
        seq_t m = (seq_t)n + (seq_t)d;      /* compute in seq_t */
        if (is_squarefree(m))
            ++(*cnt);
        return;
    }

    uint64_t p     = fac[idx];
    uint64_t val   = 1;
    for (unsigned int e = 0; e <= exp[idx]; ++e) {
        dfs_divisors(idx + 1, curr * val, fac, exp, fac_cnt, n, cnt);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    init_primes();

    uint64_t tmp  = (uint64_t)n;
    uint64_t fac[64];
    unsigned int exp[64];
    size_t fac_cnt = 0;

    for (size_t i = 0; i < PR_CNT && PR[i] * PR[i] <= tmp; ++i) {
        if (tmp % PR[i] == 0) {
            fac[fac_cnt] = PR[i];
            unsigned int e = 0;
            while (tmp % PR[i] == 0) {
                tmp /= PR[i];
                ++e;
            }
            exp[fac_cnt] = e;
            ++fac_cnt;
        }
    }
    if (tmp > 1) {            /* remaining prime factor */
        fac[fac_cnt] = tmp;
        exp[fac_cnt] = 1;
        ++fac_cnt;
    }

    seq_t count = 0;
    dfs_divisors(0, 1, fac, exp, fac_cnt, n, &count);
    return count;
}
