/*
 * A385331  a(1) = 1 and a(n) is the smallest prime factor of n-th numerator
 *          of partial sum for Liouville's constant (A145571), for n > 1.
 * https://oeis.org/A385331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = smallest prime divisor of the numerator of the n‑th
 *            partial sum of Liouville's constant (sum_{k=1..n} 10^{-k!}). For
 *            n=1 a(1)=1; for n>1 a(n) is the least prime p such that
 *            sum_{k=1..n} 10^{n! - k!} ≡ 0 (mod p). p≠2,5
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* fast modular exponentiation (base^exp mod mod) */
static uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1)
            result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1;
    }
    return result;
}

/* prime list handling */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static size_t prime_cap   = 0;

static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap)
    {
        prime_cap = prime_cap ? prime_cap * 2 : 64;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        if (!tmp) /* allocation failed – abort */
            return;
        prime_list = tmp;
    }
    prime_list[prime_cnt++] = p;
}

static void init_primes(void)
{
    if (prime_cnt == 0)
    {
        add_prime(2);
    }
}

/* simple primality test that also extends the prime list */
static bool is_prime_candidate(uint64_t n)
{
    if (n < 2)
        return false;
    /* check divisibility by stored primes */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        if (p > n / p)      /* p*p > n */
            break;
        if (n % p == 0)
            return false;
    }
    /* n is prime – store it */
    add_prime(n);
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    init_primes();

    /* use a stack array for factorials modulo (p-1) */
    const size_t MAX_STACK_N = 16384;   /* safe for n <= 10000 */
    uint64_t fact_stack[MAX_STACK_N + 1];
    uint64_t *fact_mod;
    size_t n_int = (size_t)n;

    if (n_int <= MAX_STACK_N)
        fact_mod = fact_stack;
    else
    {
        fact_mod = (uint64_t *)malloc((n_int + 1) * sizeof(uint64_t));
        if (!fact_mod)
            return (seq_t)0;         /* allocation failure */
    }

    /* iterate over primes skipping 2 and 5 */
    uint64_t candidate = 3;
    for (;; candidate += 2)
    {
        if (!is_prime_candidate(candidate))
            continue;
        if (candidate == 5)
            continue;

        uint64_t p = candidate;
        uint64_t modp1 = p - 1;

        /* factorials modulo (p-1) */
        fact_mod[0] = 1 % modp1;
        for (size_t k = 1; k <= n_int; ++k)
            fact_mod[k] = (fact_mod[k - 1] * k) % modp1;
        uint64_t nfact_mod = fact_mod[n_int];

        /* compute numerator mod p */
        uint64_t sum = 0;
        uint64_t base10 = 10 % p;
        for (size_t k = 1; k <= n_int; ++k)
        {
            uint64_t exp = nfact_mod + modp1 - fact_mod[k];
            if (exp >= modp1)
                exp -= modp1;     /* equivalently exp %= modp1 */
            uint64_t term = modpow_uint64(base10, exp, p);
            sum += term;
            if (sum >= p)
                sum -= p;
        }

        if (sum % p == 0)
        {
            if (fact_mod != fact_stack)
                free(fact_mod);
            return (seq_t)p;
        }
    }
}
