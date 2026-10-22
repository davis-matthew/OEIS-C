/*
 * A355731  Number of ways to choose a sequence of divisors, one of each
 *          element of the multiset of prime indices of n (row n of A112798).
 * https://oeis.org/A355731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} τ(index(p))^e where index(p) is the 1‑based
 *            position of prime p in the ordered list of all primes and τ(k)
 *            is the number of divisors of k
 * Range:     u8 n <= 6560, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ---------- global state for primes and divisor numbers of indices ---------- */
static uint64_t *g_primes      = NULL;   /* list of primes */
static size_t   g_prime_count  = 0;      /* number of primes stored */
static size_t   g_prime_cap    = 0;      /* allocation capacity for primes */

static bool *g_is_comp  = NULL;     /* composite markers for sieve */
static size_t g_limit    = 1;        /* highest number that has been sieved */
static size_t g_comp_cap = 0;       /* allocation capacity for is_comp */

static seq_t *g_tau      = NULL;   /* τ(k) for 1 ≤ k ≤ prime_count */
static size_t g_tau_cap  = 0;       /* capacity of the tau array */

/* ---------- auxiliary routines ---------- */

/* Compute the divisor number of a uint64_t using the current prime list. */
static seq_t tau_of_uint64(uint64_t x)
{
    seq_t res = 1;
    uint64_t tmp = x;
    for (size_t i = 0; i < g_prime_count; ++i)
    {
        uint64_t p = g_primes[i];
        if (p > tmp / p) break;            /* p² > tmp */
        int e = 0;
        while (tmp % p == 0)
        {
            tmp /= p;
            ++e;
        }
        if (e) res *= (seq_t)(e + 1);
    }
    if (tmp > 1) res *= 2;                 /* remaining prime factor */
    return res;
}

/* Multiply base^exp (exp is small uint64_t) in seq_t. */
static inline seq_t pow_seq_t(seq_t base, uint64_t exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* Binary search the 1‑based index of a prime assumed to be in the list. */
static inline size_t prime_index(uint64_t p)
{
    size_t left = 0, right = g_prime_count;
    while (left < right)
    {
        size_t mid = (left + right) >> 1;
        uint64_t val = g_primes[mid];
        if (val == p) return mid + 1;          /* 1‑based */
        if (val < p) left = mid + 1;
        else right = mid;
    }
    return 0; /* should never happen */
}

/* Incrementally extend the sieve and prime list up to 'limit' (inclusive). */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= g_limit) return;

    /* Expand composites array */
    size_t new_comp_cap = (size_t)limit + 1;
    bool *new_comp = (bool *)realloc(g_is_comp, new_comp_cap * sizeof(bool));
    if (!new_comp) exit(EXIT_FAILURE);
    g_is_comp = new_comp;
    g_comp_cap = new_comp_cap;
    memset(g_is_comp + g_limit, 0,
           (new_comp_cap - g_limit) * sizeof(bool));

    /* Mark new composites using existing primes */
    for (size_t i = 0; i < g_prime_count; ++i)
    {
        uint64_t p = g_primes[i];
        uint64_t sq = p * p;
        if (sq > limit) break;
        uint64_t start = (g_limit + p - 1) / p * p;
        if (start < sq) start = sq;
        for (uint64_t x = start; x <= limit; x += p)
            g_is_comp[(size_t)x] = true;
    }

    /* Add new primes in the new range */
    for (size_t x = g_limit + 1; x <= limit; ++x)
    {
        if (!g_is_comp[(size_t)x])
        {
            /* new prime */
            if (g_prime_count >= g_prime_cap)
            {
                size_t new_cap = g_prime_cap ? g_prime_cap * 2 : 16;
                uint64_t *new_p = (uint64_t *)realloc(g_primes,
                                                    new_cap * sizeof(uint64_t));
                if (!new_p) exit(EXIT_FAILURE);
                g_primes = new_p;
                g_prime_cap = new_cap;
            }
            g_primes[g_prime_count++] = x;

            /* Ensure tau array can hold this index */
            if (g_prime_count + 1 > g_tau_cap)
            {
                size_t new_cap = g_tau_cap ? g_tau_cap * 2 : 16;
                seq_t *new_tau = (seq_t *)realloc(g_tau,
                                                  new_cap * sizeof(seq_t));
                if (!new_tau) exit(EXIT_FAILURE);
                g_tau = new_tau;
                g_tau_cap = new_cap;
            }
            /* Compute τ for the new index (prime_count) */
            g_tau[g_prime_count] = tau_of_uint64(g_prime_count);
        }
    }

    g_limit = limit;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n <= 1) return 1;           /* a(1) = 1 */

    ensure_primes_up_to(n);

    seq_t result = 1;
    uint64_t temp = n;

    /* factor n using the prime list */
    for (size_t i = 0; i < g_prime_count; ++i)
    {
        uint64_t p = g_primes[i];
        if (p > temp / p) break;   /* p² > temp */
        if (temp % p == 0)
        {
            uint64_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            seq_t tau = g_tau[i + 1];          /* index is 1‑based */
            result *= pow_seq_t(tau, e);
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1)
    {
        size_t idx = prime_index(temp);       /* index of the prime factor */
        seq_t tau = g_tau[idx];
        result *= tau;
    }

    return result;
}
