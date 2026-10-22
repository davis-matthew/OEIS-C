/*
 * A325251  Numbers whose omega-sequence covers an initial interval of
 *          positive integers.
 * https://oeis.org/A325251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * numbers whose omega‑sequence (Ω(red^{k‑1}(n))) contains
 *            all integers * from 1 up to the maximum Ω encountered. In other
 *            words the set * {Ω(n), Ω(red(n)), …} is an initial interval [1,
 *            m] for some m
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime sieve helpers                                                */
static uint32_t *prime_list = NULL; /* 1‑indexed: prime_list[0] = 2, ... */
static size_t     prime_cnt  = 0;   /* number of primes stored */

static void build_primes(void)
{
    if (prime_list != NULL) return;       /* already built */

    const size_t LIMIT = 200000;          /* large enough for all needed n */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;

    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    /* count primes */
    size_t count = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++count;

    prime_list = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!prime_list) { free(is_prime); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint32_t)i;

    prime_cnt = count;
    free(is_prime);
}

/* ------------------------------------------------------------------ */
/* Factorisation returning Omega and exponents.
 * exps must have space for at least 64 exponents.
 * cnt = number of distinct primes,
 * omega = total number of prime factors (with multiplicity).           */
static void factor_and_omega(uint64_t n, uint32_t *exps, size_t *cnt, uint64_t *omega)
{
    *cnt = 0;
    *omega = 0;
    uint64_t t = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= t; ++i)
    {
        uint32_t p = prime_list[i];
        uint32_t e = 0;
        while (t % p == 0)
        {
            ++e;
            t /= p;
        }
        if (e > 0)
        {
            exps[(*cnt)++] = e;
            (*omega) += e;
        }
    }
    if (t > 1)   /* remaining prime factor */
    {
        exps[(*cnt)++] = 1;
        (*omega) += 1;
    }
}

/* ------------------------------------------------------------------ */
/* red(n): product of prime(e) over all exponents e of n’s factorisation */
static uint64_t red_func(uint64_t n)
{
    if (n <= 1) return 1;

    uint32_t exps[64];
    size_t cnt;
    uint64_t omega_dummy;
    factor_and_omega(n, exps, &cnt, &omega_dummy);

    uint64_t prod = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        /* exps[i] >= 1; primes[exps[i]-1] gives prime(exps[i]) */
        prod *= (uint64_t)prime_list[exps[i] - 1];
    }
    return prod;
}

/* ------------------------------------------------------------------ */
/* Test whether n belongs to the sequence */
static bool is_member(uint64_t n)
{
    if (n == 1) return true;   /* 1 is always a member */

    bool seen[64] = { false };
    uint64_t max_seen = 0;
    uint64_t current = n;

    while (true)
    {
        uint32_t exps[64];
        size_t cnt;
        uint64_t omega;
        factor_and_omega(current, exps, &cnt, &omega);

        if (omega > 0 && omega < sizeof(seen))
        {
            seen[omega] = true;
            if (omega > max_seen) max_seen = omega;
        }

        uint64_t next = red_func(current);
        if (next == current) break;  /* fixed point reached */
        current = next;
    }

    /* check that all 1..max_seen are seen */
    for (uint64_t k = 1; k <= max_seen; ++k)
        if (!seen[k]) return false;
    return true;
}

/* ------------------------------------------------------------------ */
/* Cached sequence storage                                               */
static seq_t *seq_cache = NULL;   /* array of terms so far */
static size_t seq_cache_len = 0;  /* number of cached terms */
static uint64_t next_candidate = 1; /* next integer to test */

/* ------------------------------------------------------------------ */
/* Public function required by platform                                       */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 for safety */

    /* ensure primes available */
    if (prime_list == NULL) build_primes();

    /* extend cache until we have the n-th term */
    while (seq_cache_len < (size_t)n)
    {
        if (is_member(next_candidate))
        {
            seq_t *tmp = (seq_t *)realloc(seq_cache,
                                          (seq_cache_len + 1) * sizeof(seq_t));
            if (!tmp)   /* allocation failure */
            {
                free(seq_cache);
                seq_cache = NULL;
                seq_cache_len = 0;
                return (seq_t)0;
            }
            seq_cache = tmp;
            seq_cache[seq_cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return seq_cache[n - 1];
}
