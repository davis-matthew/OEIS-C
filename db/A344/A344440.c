/*
 * A344440  a(n) = n + A061020(n).
 * https://oeis.org/A344440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 9772, u32 n <= 9772, u64 n <= 9772,
 *            u128 n <= 9772, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(n) = n + A061020(n)
 *  where A061020(n) = λ(n) * Σ_{d|n} λ(d) * (n/d)
 *  and λ(k) is the Liouville function: λ(k) = (-1)^{Ω(k)},
 *  Ω(k) being the total number of prime factors of k counted with multiplicity.
 *  Thus a(n) = n + λ(n) * Σ_{d|n} λ(d) * (n/d).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static bool primes_initialized = false;
static uint32_t primes[1229];
static size_t prime_cnt = 0;

/* Simple sieve of Eratosthenes up to 10000 */
static void init_primes(void) {
    if (primes_initialized) return;
    const uint32_t LIMIT = 10000;
    bool is_prime[LIMIT + 1];
    for (uint32_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint32_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (uint32_t m = p * p; m <= LIMIT; m += p) {
                is_prime[m] = false;
            }
        }
    }
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            primes[prime_cnt++] = i;
        }
    }
    primes_initialized = true;
}

/* Helper structure for a divisor and parity of Ω(divisor) */
struct Div {
    uint64_t val;
    unsigned parity;
};

seq_t a(idx_t n) {
    if (n == 0) {
        /* Undefined for n=0; return 0 */
        return (seq_t)0;
    }

    init_primes();

    /* Factorization of n */
    uint32_t rem = (uint32_t)n;          /* n fits in 32 bits for the intended usage */
    uint32_t factors[20];
    uint32_t exps[20];
    size_t nf = 0;
    uint32_t total_omega = 0;

    for (size_t i = 0; i < prime_cnt && (uint32_t)(primes[i] * primes[i]) <= rem; ++i) {
        uint32_t p = primes[i];
        if (rem % p == 0) {
            uint32_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
                ++total_omega;
            }
            factors[nf] = p;
            exps[nf++] = e;
        }
    }
    if (rem > 1) {
        /* remaining prime factor > sqrt(original n) */
        factors[nf] = rem;
        exps[nf] = 1;
        ++total_omega;
        ++nf;
    }

    /* Generate all divisors with their Ω parity */
    struct Div divisors[2000];           /* enough for n <= 10000 */
    size_t div_cnt = 1;
    divisors[0].val = 1;
    divisors[0].parity = 0;

    for (size_t idx = 0; idx < nf; ++idx) {
        uint32_t p = factors[idx];
        uint32_t e = exps[idx];
        size_t base_cnt = div_cnt;
        uint64_t pow = p;
        for (uint32_t exp = 1; exp <= e; ++exp, pow *= p) {
            uint32_t parity = exp & 1;
            for (size_t j = 0; j < base_cnt; ++j) {
                struct Div nd;
                nd.val = divisors[j].val * pow;
                nd.parity = divisors[j].parity ^ parity;
                divisors[div_cnt++] = nd;
            }
        }
    }

    /* Compute the sum Σ λ(d) * (n/d) */
    int64_t sum = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        int64_t term = (int64_t)n / divisors[i].val;
        if (divisors[i].parity & 1) {
            sum -= term;
        } else {
            sum += term;
        }
    }

    /* λ(n) */
    int dir = (total_omega & 1) ? -1 : 1;
    int64_t A = dir * sum;

    /* a(n) = n + A */
    int64_t result = (int64_t)n + A;

    return (seq_t)result;
}
