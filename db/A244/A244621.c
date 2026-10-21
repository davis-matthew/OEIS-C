/*
 * A244621  (First arithmetic derivative of primorials) read mod 12.
 * https://oeis.org/A244621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)#)' mod 12 (first arithmetic derivative of the
 *            n‑th primorial, modulo 12)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The function is declared externally by the build system.  The 
 * definitions of seq_t and idx_t are provided by the compiler. */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Prime table ------------------------------------------------------- */
/* ------------------------------------------------------------------ */

/* A simple pre‑computed table of primes.  The table is allocated lazily
 * and grows if required.  The primes are stored 0‑based; prime_tbl[0] = 2. */
static idx_t *prime_tbl = NULL;
static idx_t  prime_cnt = 0;

/* Sieve limit for the standard build (enough for the first 10 000 primes). */
#define SIEVE_LIMIT 2000000UL

static void init_prime_table(void)
{
    if (prime_tbl != NULL)
        return;  /* already initialised */

    /* Allocate a bitset for the sieve.  In C++ we must cast the void*. */
    unsigned char *is_comp = (unsigned char *)malloc(SIEVE_LIMIT + 1ull);
    if (!is_comp)
        return;   /* Allocation failed – give up silently. */

    for (size_t i = 0; i <= SIEVE_LIMIT; ++i)
        is_comp[i] = 0;
    is_comp[0] = is_comp[1] = 1;

    for (unsigned i = 2; (unsigned long long)i * i <= SIEVE_LIMIT; ++i)
    {
        if (!is_comp[i])
        {
            for (unsigned j = i * i; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = 1;
        }
    }

    /* Allocate the prime array; over‑estimate to avoid re‑allocation. */
    prime_tbl = (idx_t *)malloc(200000 * sizeof(idx_t));
    if (!prime_tbl) {
        free(is_comp);
        return;
    }

    for (unsigned i = 2; i <= SIEVE_LIMIT; ++i)
    {
        if (!is_comp[i])
            prime_tbl[prime_cnt++] = (idx_t)i;
    }

    free(is_comp);
}

/* ------------------------------------------------------------------ */
/* Cache for a(n) --------------------------------------------------- */
/* ------------------------------------------------------------------ */

static seq_t *a_cache      = NULL;  /* a_cache[0] holds a(1) */
static size_t a_cache_cap  = 0;     /* allocated capacity                 */
static idx_t  a_cache_len  = 0;     /* number of values currently stored  */

/* Current state for the last computed n: derivative D and primorial P
 * (both modulo 12).  These are updated whenever the cache is extended. */
static seq_t D_prev = 0;
static seq_t P_prev = 0;

/* Make sure the cache array has at least 'need' capacity. */
static void ensure_cache_capacity(idx_t need)
{
    if (a_cache_cap >= need)
        return;

    while (a_cache_cap < need) {
        size_t new_cap = (a_cache_cap == 0) ? 20000 : a_cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp)   /* allocation failed – give up silently */
            return;
        a_cache   = tmp;
        a_cache_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Main function ----------------------------------------------------- */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    /* Ensure primes and cache are set up. */
    init_prime_table();
    ensure_cache_capacity(n);

    /* Initialise the first entry if the cache is empty. */
    if (a_cache_len == 0) {
        D_prev    = 1;                    /* (2#)' = 1 */
        P_prev    = 2 % 12;               /* 2 mod 12 = 2 */
        a_cache[0] = D_prev;              /* a(1) = 1 */
        a_cache_len = 1;
    }

    /* Extend the cache up to n. */
    for (idx_t k = a_cache_len + 1; k <= n; ++k) {
        /* prime number p_k = prime_tbl[k-1] */
        seq_t p_mod12 = prime_tbl[k - 1] % 12;
        seq_t new_D   = (p_mod12 * D_prev + P_prev) % 12;
        seq_t new_P   = (P_prev * p_mod12) % 12;

        a_cache[k - 1] = new_D;
        D_prev = new_D;
        P_prev = new_P;
        ++a_cache_len;
    }

    return a_cache[n - 1];
}
