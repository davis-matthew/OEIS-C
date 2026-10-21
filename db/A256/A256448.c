/*
 * A256448  a(n) = A250474(n+1) - A250477(n).
 * https://oeis.org/A256448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = φ(p_{n+1}², n) − φ(p_n · p_{n+1}, n−1) where φ(x, a) is
 *            the number of integers ≤ x not divisible by the first a primes.
 *            (The φ function is implemented with memoisation.)
 * Range:     i8 n <= 29, i16 n <= 564, i32 n <= 564, i64 n <= 564,
 *            i128 n <= 564, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* ---------  GLOBAL PRIME TABLE (up to prime(10002))  ----------------- */
/* --------------------------------------------------------------------- */
static uint32_t *primes = NULL;   /* 0‑based array: primes[0] = 2 = prime(1) */
static size_t prime_count = 0;    /* number of primes stored */
static bool primes_initialised = false;

/* sieve limit: exceeds prime(10002) ≈ 104743 */
#define PRIME_SIEVE_LIMIT 200000U

static void init_primes(void)
{
    if (primes_initialised) return;
    primes_initialised = true;

    size_t max = PRIME_SIEVE_LIMIT;
    bool *is_prime = (bool *)malloc(max + 1);
    if (!is_prime) exit(1);
    memset(is_prime, true, max + 1);
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= max; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= max; j += i)
                is_prime[j] = false;

    /* count primes */
    prime_count = 0;
    for (size_t i = 2; i <= max; ++i)
        if (is_prime[i]) prime_count++;

    /* allocate prime array */
    primes = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!primes) exit(1);
    size_t idx = 0;
    for (size_t i = 2; i <= max; ++i)
        if (is_prime[i]) primes[idx++] = (uint32_t)i;

    free(is_prime);
}

static inline uint32_t get_prime(size_t k)
/* Returns prime(k+1) with k 0‑based (primes[0] = prime(1)) */
{
    return primes[k];
}

/* --------------------------------------------------------------------- */
/* ----------  PHI FUNCTION WITH MEMOISATION (C)  --------------------- */
/* --------------------------------------------------------------------- */
typedef struct PHIEntry {
    uint64_t x;
    uint32_t a;
    seq_t val;
    struct PHIEntry *next;
} PHIEntry;

/* hash table size – power of two for fast masking */
#define PHI_HASH_SIZE (1U << 20)   /* 1,048,576 entries */
static PHIEntry *phi_hash_table[PHI_HASH_SIZE];
static bool phi_hash_initialised = false;

/* linear probing on a chain – simple separate‑chaining hash */
static void init_phi_hash(void)
{
    if (phi_hash_initialised) return;
    phi_hash_initialised = true;
    for (size_t i = 0; i < PHI_HASH_SIZE; ++i)
        phi_hash_table[i] = NULL;
}

/* simple multiplicative hash */
static inline size_t phi_hash_key(uint64_t x, uint32_t a)
{
    return (size_t)((x ^ ((uint64_t)a << 32)) & (PHI_HASH_SIZE - 1));
}

/* lookup if present; otherwise NULL */
static PHIEntry *phi_find(uint64_t x, uint32_t a)
{
    size_t idx = phi_hash_key(x, a);
    for (PHIEntry *e = phi_hash_table[idx]; e != NULL; e = e->next)
        if (e->x == x && e->a == a)
            return e;
    return NULL;
}

/* insert new entry into hash table */
static void phi_insert(uint64_t x, uint32_t a, seq_t val)
{
    size_t idx = phi_hash_key(x, a);
    PHIEntry *e = (PHIEntry *)malloc(sizeof(PHIEntry));
    if (!e) exit(1);
    e->x = x;
    e->a = a;
    e->val = val;
    e->next = phi_hash_table[idx];
    phi_hash_table[idx] = e;
}

/* recursive φ‑function with memoisation */
static seq_t phi_cached(uint64_t x, uint32_t a)
{
    if (a == 0) return (seq_t)x;
    if (a == 1) return (seq_t)((x + 1U) >> 1U);  /* (x+1)/2 */

    /* look up in cache */
    PHIEntry *e = phi_find(x, a);
    if (e != NULL) return e->val;

    /* compute recursively */
    seq_t res = phi_cached(x, a - 1U);
    seq_t sub = phi_cached(x / (uint64_t)primes[a - 1U], a - 1U);
    res -= sub;

    /* store result */
    phi_insert(x, a, res);
    return res;
}

/* --------------------------------------------------------------------- */
/* -------------------------  MAIN SEQUENCE  --------------------------- */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined below offset */

    init_primes();
    init_phi_hash();

    /* indices in primes array are 0‑based */
    uint64_t p_n   = (uint64_t)primes[(size_t)(n - 1)];     /* prime(n)  */
    uint64_t p_np1 = (uint64_t)primes[(size_t)n];          /* prime(n+1) */
    uint64_t p_np2 = (uint64_t)primes[(size_t)(n + 1)];    /* prime(n+2) */

    /* limits for the two counts */
    uint64_t limit1 = p_np1 * p_np1;           /* p_{n+1}^2          */
    uint64_t limit2 = p_n * p_np1;             /* p_n * p_{n+1}      */

    /* φ‑values */
    seq_t phi1 = phi_cached(limit1, (uint32_t)n);            /* φ(limit1, n)          */
    seq_t phi2 = phi_cached(limit2, (uint32_t)(n - 1));     /* φ(limit2, n-1)        */

    return phi1 - phi2;
}
