/*
 * A349385  Dirichlet convolution of A048673 with the Dirichlet inverse of
 *          A003961, where A003961 is fully multiplicative with a(p) =
 *          nextprime(p), and A048673(n) = (1+A003961(n))/2.
 * https://oeis.org/A349385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n} A048673(n/d) * A346234(d) * where A048673(m) = (1
 *            + Π nextprime(p)^e) / 2 * and A346234(d) = 1 if d=1, * =
 *            (-1)^ω(d) * Π nextprime(p) if d is squarefree, * = 0 otherwise *
 *            (nextprime(p) is the least prime > p) * * This implementation
 *            uses the fact that the Dirichlet inverse of * A0...
 * Range:     i8 n <= 23, i16 n <= 5519, i32 n <= 9083, i64 n <= 9083,
 *            i128 n <= 9083, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ----------  Sieve for primes up to 20000  ---------- */
#define SIEVE_LIMIT          20000
#define MAX_PRIME_COUNT      2500

static unsigned char comp[SIEVE_LIMIT + 1];
static uint32_t primes[MAX_PRIME_COUNT];
static uint32_t nextPrime[MAX_PRIME_COUNT];
static size_t primeCnt = 0;
static bool initialized = false;

static void init_sieve(void)
{
    if (initialized) return;

    /* mark composites */
    comp[0] = comp[1] = 1;
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (!comp[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                comp[j] = 1;

    /* collect primes */
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!comp[i]) {
            primes[primeCnt++] = (uint32_t)i;
        }

    /* next prime table */
    for (size_t i = 0; i + 1 < primeCnt; ++i)
        nextPrime[i] = primes[i + 1];
    nextPrime[primeCnt - 1] = 0;           /* no succeeding prime in the table */

    initialized = true;
}

/* ----------  Factorisation of n <= 10000  ---------- */
static void factorise(idx_t n,
                      idx_t *pf, int *exp, size_t *k)
{
    *k = 0;
    for (size_t i = 0; i < primeCnt && (idx_t)primes[i] * primes[i] <= n; ++i) {
        if (n % primes[i] == 0) {
            pf[(*k)] = primes[i];
            int cnt = 0;
            while (n % primes[i] == 0) {
                n /= primes[i];
                ++cnt;
            }
            exp[(*k)++] = cnt;
        }
    }
    if (n > 1) {            /* remaining prime factor > sqrt(original n) */
        pf[(*k)] = n;
        exp[(*k)++] = 1;
    }
}

/* ----------  Get next prime for a prime p  ---------- */
static uint32_t get_next_prime(idx_t p)
{
    /* p is guaranteed to be in the primes list (p <= 20000) */
    for (size_t i = 0; i < primeCnt; ++i)
        if (primes[i] == p)
            return nextPrime[i];
    /* unreachable, but provide a safe fallback */
    idx_t candidate = p + 1;
    for (;;) {
        bool isprime = true;
        for (size_t i = 0; i < primeCnt && (idx_t)primes[i] * primes[i] <= candidate; ++i)
            if (candidate % primes[i] == 0) { isprime = false; break; }
        if (isprime) return (uint32_t)candidate;
        ++candidate;
    }
}

/* ----------  Core function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined but harmless */

    init_sieve();

    /* factorise n */
    idx_t pf[15];
    int exp[15];
    size_t k;
    factorise(n, pf, exp, &k);

    /* nextprime for each prime factor */
    uint32_t q[15];
    for (size_t i = 0; i < k; ++i)
        q[i] = get_next_prime(pf[i]);

    /* baseSub = Π q[i]^exp[i]  (i.e. A003961(n)) */
    seq_t baseSub = 1;
    for (size_t i = 0; i < k; ++i)
        for (int e = 0; e < exp[i]; ++e)
            baseSub *= (seq_t)q[i];

    seq_t sum = 0;
    /* iterate over all subsets of the distinct primes of n */
    for (unsigned mask = 0; mask < (1u << k); ++mask) {
        seq_t Q = 1;          /* Π nextprime(p) over chosen primes */
        int bits = 0;
        for (size_t i = 0; i < k; ++i)
            if (mask & (1u << i)) {
                Q *= (seq_t)q[i];
                ++bits;
            }

        /* g(d) = (-1)^bits * Q , with g(1)=1 when mask==0 */
        seq_t g = (bits % 2 == 0 ? 1 : -1) * Q;

        seq_t sub = baseSub / Q;           /* A003961(n/d) */
        seq_t a048 = (sub + (seq_t)1) / (seq_t)2;   /* A048673(n/d) */

        sum += a048 * g;
    }

    return sum;
}
