/*
 * A374906  a(n) is the least number whose divisors have a mean number of
 *          divisors that equals n.
 * https://oeis.org/A374906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least integer k such that the mean number of *
 *            divisors of the divisors of k equals n. * That is, [sum_{d|k}
 *            τ(d)] / τ(k) = n, where τ is the divisor counting function. *
 *            The following implementation searches over all possible exponent
 *            * multisets e_i ≥ 0 with product \prod (e_i+2) =...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 66, u128 n <= 126,
 *            big any n
 * Helpers:   big_pow, seq_cmp, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_cmp.h"

/* Small list of primes sufficient for n ≤ 10 000.  The algorithm
 * never requires more than 20 distinct primes. */
static const uint64_t PRIMES[20] =
{
    2,3,5,7,11,13,17,19,23,29,
    31,37,41,43,47,53,59,61,67,71
};

/* Recursive enumeration of factorizations.  target is the remaining product,
 * min_f is the smallest factor we may use (to avoid duplicates),
 * left counts how many factors still need to be chosen,
 * fact holds the current list of factors,
 * len is the current position in fact, and
 * best stores the best value found so far. */
static void dfs_fact(uint64_t target, uint64_t min_f, int left,
                     uint64_t *fact, int len, seq_t *best)
{
    if (left == 0)
    {
        if (target != 1) return;          /* incomplete factorization */
        /* Build exponents e_i = f_i - 2 (ignore e_i = 0). */
        uint64_t exps[20];
        int exps_len = 0;
        for (int i = 0; i < len; ++i)
        {
            uint64_t e = fact[i] - 2;
            if (e > 0) exps[exps_len++] = e;
        }
        /* Sort exponents descending. */
        for (int i = 1; i < exps_len; ++i)
        {
            uint64_t key = exps[i];
            int j = i;
            while (j > 0 && exps[j - 1] < key)
            {
                exps[j] = exps[j - 1];
                --j;
            }
            exps[j] = key;
        }
        /* Compute k = ∏ primes[i]^exps[i] */
        seq_t val = 1;
        for (int i = 0; i < exps_len; ++i)
        {
            seq_t p = PRIMES[i];
            seq_t e = (seq_t)exps[i];
            seq_t pow = big_pow(p, e);
            val = seq_mul(val, pow);
        }
        /* Update best if smaller. */
        if (*best == 0 || seq_cmp(&val, best) < 0)
            *best = val;
        return;
    }

    /* If only one factor remains, it must be the whole target. */
    if (left == 1)
    {
        fact[len] = target;
        dfs_fact(1, target, 0, fact, len + 1, best);
        return;
    }

    /* Try all possible factors from min_f up to sqrt(target). */
    uint64_t lim = (uint64_t)sqrt((long double)target);
    for (uint64_t f = min_f; f <= lim; ++f)
    {
        if (target % f == 0)
        {
            fact[len] = f;
            dfs_fact(target / f, f, left - 1, fact, len + 1, best);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;          /* a(1) = 1 */

    seq_t best = 0;
    /* Try all possible numbers of primes r.  For n ≤ 10 000,
     * r up to 20 is sufficient. */
    for (int r = 1; r <= 20; ++r)
    {
        uint64_t target = (uint64_t)n << r;   /* n * 2^r */
        uint64_t cur[20];
        dfs_fact(target, 2, r, cur, 0, &best);
    }
    return best;
}
