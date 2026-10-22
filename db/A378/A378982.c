/*
 * A378982  a(n) = (A003961(n)-(1+sigma(n))) mod (A003961(n)-2*n), where
 *          A003961 is fully multiplicative with a(prime(i)) = prime(i+1).
 * https://oeis.org/A378982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A003961(n)-(1+sigma(n))) mod (A003961(n)-2*n)
 * Range:     u8 n <= 16, u16 n <= 1534, u32 n <= 9314, u64 n <= 9314,
 *            u128 n <= 9314, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A003961 is fully multiplicative with a(prime_i)=prime_{i+1}.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#define PRIMELIMIT 2000000          /* sufficient for n up to ~1e6 */

static bool      primes_built = false;
static size_t    prime_cnt    = 0;
static uint64_t *prime_list   = NULL;

/* Simple sieve to fill prime_list up to PRIMELIMIT.  */
static void init_primes(void)
{
    if (primes_built) return;

    bool *is_prime = (bool*)malloc((PRIMELIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (size_t i = 2; i <= PRIMELIMIT; ++i) is_prime[i] = true;

    for (size_t p = 2; p * p <= PRIMELIMIT; ++p)
        if (is_prime[p])
            for (size_t m = p * p; m <= PRIMELIMIT; m += p)
                is_prime[m] = false;

    /* count primes */
    prime_cnt = 0;
    for (size_t i = 2; i <= PRIMELIMIT; ++i)
        if (is_prime[i]) prime_cnt++;

    prime_list = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_list) exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= PRIMELIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint64_t)i;

    free(is_prime);
    primes_built = true;
}

/* Fast power for seq_t base^{exp} with exp small (unsigned). */
static inline seq_t pow_seq_i(seq_t base, unsigned exp)
{
    seq_t res = 1;
    while (exp--) res *= base;
    return res;
}

/* Find the next prime after p using binary search.  Assumes p is a prime in the list. */
static inline uint64_t next_prime(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= p) lo = mid + 1;
        else                      hi = mid;
    }
    /* lo is index of first prime > p, assume it exists */
    if (lo < prime_cnt) return prime_list[lo];
    /* Should not happen for the intended range; fallback */
    uint64_t q = p + 1;
    while (true) {
        bool prime = true;
        for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= q; ++i)
            if (q % prime_list[i] == 0) { prime = false; break; }
        if (prime) return q;
        ++q;
    }
}

/* Compute sigma(n) and A003961(n) together while factoring n. */
static void factor_and_compute(seq_t n, seq_t *out_sigma, seq_t *out_A)
{
    init_primes();

    seq_t sigma = 1;
    seq_t A     = 1;

    uint64_t temp = (uint64_t)n;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p != 0) continue;

        unsigned exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        /* sigma contribution */
        seq_t pow_p_e1 = pow_seq_i((seq_t)p, exp + 1);
        seq_t num      = pow_p_e1 - 1;
        seq_t den      = (seq_t)(p - 1);
        sigma *= num / den;

        /* A003961 contribution */
        uint64_t nxt = next_prime(p);
        seq_t pow_nxt = pow_seq_i((seq_t)nxt, exp);
        A *= pow_nxt;
    }

    if (temp > 1) {   /* remaining prime factor */
        uint64_t p = temp;
        unsigned exp = 1;

        seq_t pow_p_e1 = pow_seq_i((seq_t)p, exp + 1);
        sigma *= (pow_p_e1 - 1) / (p - 1);

        uint64_t nxt = next_prime(p);
        A *= pow_seq_i((seq_t)nxt, exp);
    }

    *out_sigma = sigma;
    *out_A = A;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety for index 0 */

    seq_t N       = (seq_t)n;
    seq_t sigma, A;
    factor_and_compute(N, &sigma, &A);

    seq_t twoN = (seq_t)2 * N;
    seq_t diff;
    if (A >= twoN) diff = A - twoN;
    else           diff = twoN - A;

    if (diff == 0) return (seq_t)0;   /* should not occur for n>=1 */

    seq_t one_plus_sigma = sigma + 1;
    seq_t rem;

    if (A >= one_plus_sigma) {
        rem = (A - one_plus_sigma) % diff;
    } else {
        seq_t diff_minus = one_plus_sigma - A;
        seq_t mod = diff_minus % diff;
        rem = (diff - mod) % diff;
    }

    return rem;
}
