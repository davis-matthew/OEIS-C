/*
 * A295798  a(n) is the number of divisors d of prime(n)^2 - 1 such that
 *          prime(n) + d is prime.
 * https://oeis.org/A295798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of prime(n)^2–1 such that prime(n)+d
 *            is prime. (OEIS A295798)
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 55, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation enumerates all divisors of (prime(n)^2−1) by
   first factoring (prime(n)−1) and (prime(n)+1) separately, merging the
   factorizations, then checking each divisor.  All helper functions
   are provided in the universal <../utils/...> headers. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/is_prime_big.h"

static void dfs_divisors(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                         size_t idx, seq_t cur, seq_t p, seq_t *res)
{
    if (idx == cnt) {
        if (is_prime_big(p + cur))
            (*res)++;
        return;
    }
    seq_t mult = cur;
    for (size_t k = 0; k <= ex[idx]; ++k) {
        dfs_divisors(pr, ex, cnt, idx + 1, mult, p, res);
        mult *= (seq_t)pr[idx];
    }
}

seq_t a(idx_t n)
{
    /* 1‑based index, retrieve the n‑th prime */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* factor (p−1) and (p+1) – they are close to each other and easy to factor */
    uint64_t pr1[64], ex1[64], pr2[64], ex2[64];
    size_t cnt1 = 0, cnt2 = 0;
    factor_u64((uint64_t)p32 - 1ULL, pr1, ex1, &cnt1);
    factor_u64((uint64_t)p32 + 1ULL, pr2, ex2, &cnt2);

    /* merge factorizations into a single list for p^2−1 */
    uint64_t pr_comb[128], ex_comb[128];
    size_t tot = 0;

    /* copy first factorization */
    for (size_t i = 0; i < cnt1; ++i) {
        pr_comb[tot] = pr1[i];
        ex_comb[tot] = ex1[i];
        ++tot;
    }

    /* merge second factorization */
    for (size_t j = 0; j < cnt2; ++j) {
        bool found = false;
        for (size_t i = 0; i < cnt1; ++i) {
            if (pr2[j] == pr_comb[i]) {
                ex_comb[i] += ex2[j];
                found = true;
                break;
            }
        }
        if (!found) {
            pr_comb[tot] = pr2[j];
            ex_comb[tot] = ex2[j];
            ++tot;
        }
    }

    /* enumerate divisors and count those that satisfy the primality test */
    seq_t result = 0;
    dfs_divisors(pr_comb, ex_comb, tot, 0, (seq_t)1, p, &result);

    return result;
}
