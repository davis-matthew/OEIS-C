/*
 * A372432  Positive integers k such that the prime indices of k are not
 *          disjoint from the binary indices of k.
 * https://oeis.org/A372432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A372432: Positive integers k such that the prime indices of k
 * are not disjoint from the binary indices of k.
 * a(n) is the n-th such integer (a(1) = 3, a(2) = 5, …).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Sieve parameters (enough for the first 10 000 terms)                */
/* ------------------------------------------------------------------ */
#define SIEVE_MAX   2000000u   /* limit for prime generation */

/* ------------------------------------------------------------------ */
/* Static data for sieve and for the sequence cache                   */
/* ------------------------------------------------------------------ */
static bool         sieve_generated = false;
static uint8_t      *is_composite   = NULL;     /* 0 = prime, 1 = composite */
static uint32_t     *prime_index    = NULL;     /* prime_index[p] = index of p (1‑based) */
static uint32_t     *prime_list     = NULL;     /* array of primes */
static size_t       prime_cnt      = 0;         /* number of primes */

static size_t       cache_cap      = 0;         /* alloced capacity of seq_cache */
static seq_t       *seq_cache      = NULL;      /* cached terms of the sequence */
static size_t       seq_found      = 0;         /* number of terms stored */
static uint32_t     search_k       = 2;         /* next integer to test */

/* ------------------------------------------------------------------ */
/* Helper functions                                                 */
/* ------------------------------------------------------------------ */

/* Allocate and generate the sieve, prime list and prime indices.  */
static void init_sieve(void)
{
    if (sieve_generated)
        return;
    sieve_generated = true;

    /* Allocate arrays */
    is_composite = (uint8_t*)calloc(SIEVE_MAX + 1, sizeof(uint8_t));
    if (!is_composite) return;           /* allocation failure */

    prime_index = (uint32_t*)calloc(SIEVE_MAX + 1, sizeof(uint32_t));
    if (!prime_index) { free(is_composite); return; }

    /* Initial capacity for prime_list */
    uint32_t prime_cap = 200000;         /* enough for 2 M */
    prime_list = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_list) { free(is_composite); free(prime_index); return; }

    /* Sieve of Eratosthenes */
    for (uint32_t i = 2; i <= SIEVE_MAX; ++i) {
        if (!is_composite[i]) {
            /* i is prime */
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                uint32_t *tmp = (uint32_t*)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!tmp) return;       /* allocation failure */
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = i;
            prime_index[i] = (uint32_t)prime_cnt;   /* 1‑based */

            /* Mark multiples */
            if ((uint64_t)i * i <= SIEVE_MAX) {
                for (uint32_t j = i * i; j <= SIEVE_MAX; j += i)
                    is_composite[j] = 1;
            }
        }
    }
}

/* Test whether k satisfies the sequence condition.  */
static bool k_good(uint32_t k)
{
    /* Build mask of binary indices: bit i (0‑based) set if LSB+ i is set */
    uint64_t bin_mask = 0;
    uint32_t tmp = k;
    uint32_t pos = 0;
    while (tmp) {
        if (tmp & 1u)
            bin_mask |= (1ULL << pos);
        tmp >>= 1;
        ++pos;
    }

    /* Factor k using our prime list */
    uint32_t num = k;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= num; ++i) {
        uint32_t p = prime_list[i];
        if (num % p == 0) {
            uint32_t pi = prime_index[p];      /* index of prime p (1‑based) */
            if (pi != 0 && ((bin_mask >> (pi - 1)) & 1ULL))
                return true;
            while (num % p == 0)
                num /= p;
        }
    }
    /* Remaining prime factor > sqrt(k) */
    if (num > 1) {
        uint32_t pi = prime_index[num];
        if (pi != 0 && ((bin_mask >> (pi - 1)) & 1ULL))
            return true;
    }
    return false;
}

/* Ensure seq_cache has room for at least 'needed' elements.          */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) return;   /* allocation failure – keep old pointer */
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* undefined, but safe */

    init_sieve();

    /* Generate terms until we have at least n of them */
    if (seq_found < (size_t)n) {
        ensure_cache_capacity(n);
        while (seq_found < (size_t)n) {
            if (search_k > SIEVE_MAX) {
                /* Our sieve limit was insufficient – should not happen for n <= 10000. */
                /* Abort further search to avoid infinite loop. */
                break;
            }
            if (k_good(search_k)) {
                seq_cache[seq_found++] = (seq_t)search_k;
            }
            ++search_k;
        }
    }

    if (seq_found >= (size_t)n)
        return seq_cache[n - 1];
    return 0;    /* in case of failure */
}
