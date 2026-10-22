/*
 * A394398  Number of integers k, 1 <= k < n, such that n divides k^2-1 and k
 *          divides n^2-1.
 * https://oeis.org/A394398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of k, 1 <= k < n, such that n | k²-1 and k | n²-1
 *            Computed by enumerating the divisors of n²-1 that are less than
 *            n and checking whether k² ≡ 1 (mod n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_PRIME_LIMIT 20000   /* suffices for n up to ~10⁴ */

static uint64_t primes_global[2500];
static size_t primes_cnt = 0;

/* Simple sieve to populate primes_global */
static void init_primes(void)
{
    if (primes_cnt != 0) return;
    bool sieve[MAX_PRIME_LIMIT + 1];
    for (int i = 0; i <= MAX_PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= MAX_PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                sieve[j] = false;
        }
    }
    for (int i = 2; i <= MAX_PRIME_LIMIT; ++i)
        if (sieve[i]) primes_global[primes_cnt++] = i;
}

/* Recursively generate all divisors of M from its prime factorisation
   and count those that satisfy the conditions. */
static void gen_divs(size_t idx, uint64_t prod,
                     const uint64_t *factors, const uint64_t *exps,
                     size_t num, idx_t n, seq_t *cnt)
{
    if (idx == num) {
        if (prod < (uint64_t)n) {
            uint64_t mod = (prod * prod) % (uint64_t)n;
            if (mod == 1)
                (*cnt)++;
        }
        return;
    }
    uint64_t p   = factors[idx];
    uint64_t e   = exps[idx];
    uint64_t cur = 1;
    for (uint64_t i = 0; i <= e; ++i) {
        gen_divs(idx + 1, prod * cur, factors, exps, num, n, cnt);
        cur *= p;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    init_primes();

    uint64_t N   = (uint64_t)n;
    uint64_t M   = N * N - 1;                    /* n²‑1, positive */
    uint64_t tmp = M;

    /* factorise M using the precomputed primes */
    uint64_t fac_primes[64];
    uint64_t fac_exps[64];
    size_t   fac_cnt = 0;

    for (size_t i = 0; i < primes_cnt && primes_global[i] * primes_global[i] <= tmp; ++i) {
        uint64_t p = primes_global[i];
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            fac_primes[fac_cnt] = p;
            fac_exps[fac_cnt]   = e;
            fac_cnt++;
        }
    }
    if (tmp > 1) {                   /* remaining prime factor */
        fac_primes[fac_cnt] = tmp;
        fac_exps[fac_cnt]   = 1;
        fac_cnt++;
    }

    seq_t count = 0;
    gen_divs(0, 1, fac_primes, fac_exps, fac_cnt, n, &count);
    return count;
}
