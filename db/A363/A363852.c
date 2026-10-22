/*
 * A363852  Number of divisors of 7*n-5 of form 7*k+1.
 * https://oeis.org/A363852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of 7*n-5 with d ≡ 1 (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Simple prime sieve to generate all primes up to MAX_PRIME. */
#define MAX_PRIME 1000
static uint64_t primes[MAX_PRIME];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt > 0) return;
    bool is_composite[MAX_PRIME] = {false};
    for (uint64_t i = 2; i < MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            if (i * i < MAX_PRIME)
                for (uint64_t j = i * i; j < MAX_PRIME; j += i)
                    is_composite[j] = true;
        }
    }
}

/* Recursive enumeration of all divisors of a number given its factorisation.
   Count those that are ≡ 1 (mod 7). */
static void dfs(const uint64_t *pr, const int *exp, size_t cnt,
                size_t idx, uint64_t cur, seq_t *result)
{
    if (idx == cnt) {
        if (cur % 7ULL == 1ULL) (*result)++;
        return;
    }
    uint64_t val = 1ULL;
    for (int e = 0; e <= exp[idx]; ++e) {
        dfs(pr, exp, cnt, idx + 1, cur * val, result);
        val *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1 */
    if (n == 0) return 0;

    init_primes();

    /* m = 7*n - 5 */
    uint64_t m = 7ULL * (uint64_t)n - 5ULL;

    /* Factorise m */
    uint64_t tmp = m;
    uint64_t factor[64];
    int expo[64];
    size_t fcnt = 0;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            factor[fcnt] = primes[i];
            int e = 0;
            while (tmp % primes[i] == 0) {
                tmp /= primes[i];
                ++e;
            }
            expo[fcnt++] = e;
        }
    }
    if (tmp > 1) {
        factor[fcnt] = tmp;
        expo[fcnt] = 1;
        ++fcnt;
    }

    /* Count admissible divisors */
    seq_t count = 0;
    dfs(factor, expo, fcnt, 0, 1ULL, &count);

    return count;
}
