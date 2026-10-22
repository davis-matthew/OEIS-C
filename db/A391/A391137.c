/*
 * A391137  Numbers k such that the divisors of k^2 + 1 are all of the form
 *          m^2 + 1.
 * https://oeis.org/A391137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A391137: numbers k such that each divisor d of k^2+1 satisfies
   d‑1 is a perfect square (i.e. d = m^2+1 for some integer m).  The
   function generates the sequence by testing successive k in increasing
   order and memoizes the results for arbitrary ordering of calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* --------------------------------------------------------------------- */
/* Small prime sieve (up to a fixed limit sufficient for k up to ~1e5). */
/* --------------------------------------------------------------------- */
#define PRIME_SIEVE_LIMIT 200000U

static uint32_t *primes = NULL;
static size_t primes_count = 0;
static bool primes_initialised = false;

static void
init_primes(void)
{
    bool *is_comp = (bool *)calloc(PRIME_SIEVE_LIMIT + 1U, sizeof(bool));
    if (!is_comp) return;  /* allocation failure ignored; sequence will fail */

    size_t cap = PRIME_SIEVE_LIMIT / 10U + 1U;
    primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!primes) return;

    primes_count = 0;
    for (uint32_t i = 2U; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (primes_count == cap) {
                cap <<= 1U;
                uint32_t *tmp = (uint32_t *)realloc(primes, cap * sizeof(uint32_t));
                if (!tmp) break;
                primes = tmp;
            }
            primes[primes_count++] = i;
            if ((uint64_t)i * i <= PRIME_SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    primes_initialised = true;
}

/* --------------------------------------------------------------------- */
/* Integer square root for 64‑bit unsigned.                              */
/* --------------------------------------------------------------------- */
static inline uint64_t
isqrt_u64(uint64_t n)
{
    uint64_t r = (uint64_t)sqrt((double)n);
    while ((r + 1ULL) * (r + 1ULL) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/* --------------------------------------------------------------------- */
/* Test whether k satisfies the defining property (k is 64‑bit).         */
/* --------------------------------------------------------------------- */
static bool
k_is_valid(uint64_t k)
{
    uint64_t n = k * k + 1ULL;            /* k^2+1 must fit into 64‑bit */
    uint64_t tmp = n;

    /* Factorisation of n.  Store prime factors and exponents.          */
    uint64_t primes_f[64];
    int exps[64];
    size_t pc = 0U;

    for (size_t i = 0U; i < primes_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp;
         ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0U) {
            int e = 0;
            while (tmp % p == 0U) {
                tmp /= p;
                ++e;
            }
            primes_f[pc] = p;
            exps[pc] = e;
            ++pc;
        }
    }
    if (tmp > 1ULL) {
        primes_f[pc] = tmp;
        exps[pc] = 1;
        ++pc;
    }

    /* Generate all divisors of n.                                         */
    uint64_t divs[2048];
    size_t dc = 1U;
    divs[0] = 1ULL;

    for (size_t i = 0U; i < pc; ++i) {
        uint64_t p = primes_f[i];
        int e   = exps[i];
        size_t olddc = dc;
        uint64_t pow = 1ULL;
        for (int j = 1; j <= e; ++j) {
            pow *= p;
            for (size_t k = 0U; k < olddc; ++k) {
                if (dc >= 2048U) return false;  /* overflow safeguard */
                divs[dc++] = divs[k] * pow;
            }
        }
    }

    /* Check the property for every divisor.                               */
    for (size_t i = 0U; i < dc; ++i) {
        uint64_t d = divs[i];
        uint64_t v = d - 1ULL;               /* d>=1, so v>=0 */
        uint64_t r = isqrt_u64(v);
        if (r * r != v) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Ensure a dynamic array has enough capacity.                            */
/* --------------------------------------------------------------------- */
static void
ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0U) ? 16U : (*cap << 1U);
        while (newcap < need) newcap <<= 1U;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *arr = tmp ? tmp : *arr;            /* retain old on fail */
        *cap = newcap;
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function.                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t len = 0U;
    static size_t cap = 0U;
    static uint64_t next_test = 0ULL;   /* next k to examine */

    if (!primes_initialised) init_primes();

    size_t target = (size_t)n;
    while (len < target) {
        if (k_is_valid(next_test)) {
            ensure_capacity(&seq, &cap, len + 1U);
            seq[len++] = (seq_t)next_test;
        }
        ++next_test;
    }
    return seq[target - 1U];
}
