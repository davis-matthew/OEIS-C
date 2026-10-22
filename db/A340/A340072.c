/*
 * A340072  a(n) = phi(x) / gcd(x-1, phi(x)), where x = A003961(n), i.e., n
 *          with its prime factorization shifted one step towards larger
 *          primes.
 * https://oeis.org/A340072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(x) / gcd(x‑1 , φ(x)) where x = ∏ next_prime(p)^e over
 *            the prime factorization p^e of n
 * Range:     u8 n <= 55, u16 n <= 3455, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/*----- small primes sieve (up to LIMIT) -----*/
#define PRIME_LIMIT 25000
static uint64_t primes[3000];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt != 0)
        return;
    bool comp[PRIME_LIMIT + 1] = { false };
    prime_cnt = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!comp[i]) {
            primes[prime_cnt++] = i;
            if (i * i <= PRIME_LIMIT)
                for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                    comp[j] = true;
        }
    }
}

/*----- find the next prime after p -----*/
static uint64_t next_prime_uint64(uint64_t p)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (primes[i] == p) {
            if (i + 1 < prime_cnt)
                return primes[i + 1];
            /* fall through to generic search */
            break;
        }
    }
    /* generic search – patterns for values beyond precomputed range */
    uint64_t cand = p + 1;
    /* ensure cand stays odd if >2 */
    if (cand > 2 && cand % 2 == 0)
        cand++;
    while (1) {
        bool isprime = true;
        for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime)
            return cand;
        cand += 2;
    }
}

/*----- factorization of n, up to ~2^64 — only needed for n <= PRIME_LIMIT^2 -----*/
static size_t factorize_n(uint64_t n, uint64_t *pr, int *exps)
{
    size_t cnt = 0;
    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= m; ++i) {
        if (m % primes[i] == 0) {
            int e = 0;
            do {
                m /= primes[i];
                ++e;
            } while (m % primes[i] == 0);
            pr[cnt]   = primes[i];
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {
        pr[cnt]   = m;
        exps[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/*----- main sequence function -----*/
seq_t a(idx_t n)
{
    /* index is 1‑based */
    if (n == (idx_t)0)  /* undefined but just guard */
        return (seq_t)0;
    if (n == (idx_t)1)
        return (seq_t)1; /* A003961(1) = 1, φ(1)=1, gcd(0,1)=1 */

    init_primes();

    uint64_t np = (uint64_t)n;
    uint64_t fac[64];
    int    exp[64];
    size_t fcnt = factorize_n(np, fac, exp);

    seq_t x = (seq_t)1;
    seq_t phi = (seq_t)1;

    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t p = fac[i];
        int e = exp[i];
        uint64_t q = next_prime_uint64(p);

        seq_t pow_q_e = (seq_t)1;
        for (int k = 0; k < e; ++k)
            pow_q_e *= (seq_t)q;

        seq_t pow_q_e_minus1 = (e > 1) ? (pow_q_e / (seq_t)q) : (seq_t)1;

        x   *= pow_q_e;
        phi *= ((seq_t)(q - 1) * pow_q_e_minus1);
    }

    seq_t g = gcd(x - (seq_t)1, phi);
    return phi / g;
}
