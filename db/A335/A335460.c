/*
 * A335460  Number of (1,2,1) or (2,1,2)-matching permutations of the prime
 *          indices of n.
 * https://oeis.org/A335460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * let k = total number of prime factors of n counted with
 *            multiplicity, * let e_i = multiplicity of the i‑th distinct
 *            prime factor, * let m = number of distinct prime factors. * a(n)
 *            = k! / (Π_i e_i!) – m! . * (If k < 3 the expression evaluates to
 *            0.)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/factorial.h"   /* provides: seq_t factorial(idx_t n); */

static uint64_t *prime_list = NULL;  /* list of primes up to current_limit */
static size_t    prime_cnt   = 0;    /* number of primes in prime_list   */
static idx_t     current_limit = 0;   /* maximum n for which primes were built */

/* Build an array of all primes <= limit using a simple sieve. */
static void build_primes_upto(idx_t limit)
{
    if (limit <= current_limit) return;

    /* free any old list */
    free(prime_list);
    prime_list = NULL;
    prime_cnt  = 0;
    current_limit = limit;

    /* allocate sieve */
    bool *is_prime = (bool*)malloc((size_t)(limit + 1));
    if (!is_prime) return;
    memset(is_prime, true, (size_t)(limit + 1));
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    /* store primes */
    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    prime_cnt  = cnt;
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[pos++] = (uint64_t)i;

    free(is_prime);
}

/* Factor n, returning the total exponent count k,
 * the number of distinct prime factors m,
 * and storing each exponent in exps[] (size m).  */
static void factor(uint64_t n, size_t *k_out, size_t *m_out, uint64_t *exps)
{
    *k_out = 0;
    *m_out = 0;
    idx_t temp = (idx_t)n;
    for (size_t i = 0; i < prime_cnt && (idx_t)prime_list[i] * (idx_t)prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            uint64_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            exps[(*m_out)++] = cnt;
            *k_out += (size_t)cnt;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        exps[(*m_out)++] = 1;
        *k_out += 1;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    build_primes_upto((idx_t)n);          /* ensure we have all primes <= n */

    uint64_t exps[64];   /* at most 64 distinct prime factors for 64‑bit n */
    size_t   k, m;       /* total exponents and distinct prime count */

    factor((uint64_t)n, &k, &m, exps);

    /* compute the total number of distinct permutations:
       k! / (Π e_i!) */
    seq_t total = factorial((idx_t)k);
    for (size_t i = 0; i < m; ++i)
        total /= factorial((idx_t)exps[i]);

    /* permutations that avoid the pattern are simply permutations of the
       distinct blocks: m!  */
    seq_t avoid = factorial((idx_t)m);

    seq_t result = total - avoid;   /* always non‑negative by construction */
    return result;
}
