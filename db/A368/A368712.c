/*
 * A368712  The maximal exponent in the prime factorization of the cubefree
 *          numbers.
 * https://oeis.org/A368712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n=1; otherwise a(n)=1 if the n-th cubefree number is
 *            squarefree, a(n)=2 if it contains a square factor (but is still
 *            cubefree)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* static storage for computed sequence values */
static seq_t *seq_arr = NULL;   /* array of a(k) values, 0‑based index */
static size_t  seq_cap = 0;     /* allocated capacity */
static size_t  seq_size = 0;    /* number of values stored */
static uint64_t last_checked = 0; /* last integer examined for cubefreeness */

/* static storage for primes (needed for cube and square checks) */
static bool *is_prime_arr  = NULL;
static uint64_t *prime_list = NULL;
static size_t  prime_cap   = 0; /* number of primes stored */
static bool primes_initialized = false;

#define PRIME_MAX 200000  /* sieve upper bound – enough for n≈10000 */

static void init_primes(void)
{
    is_prime_arr = (bool*)malloc((PRIME_MAX + 1) * sizeof(bool));
    if (!is_prime_arr) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= PRIME_MAX; ++i) is_prime_arr[i] = true;
    is_prime_arr[0] = is_prime_arr[1] = false;

    for (size_t i = 2; i * i <= PRIME_MAX; ++i)
        if (is_prime_arr[i])
            for (size_t j = i * i; j <= PRIME_MAX; j += i)
                is_prime_arr[j] = false;

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_MAX; ++i)
        if (is_prime_arr[i]) ++cnt;

    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_MAX; ++i)
        if (is_prime_arr[i]) prime_list[idx++] = (uint64_t)i;
    prime_cap = cnt;
    primes_initialized = true;
}

static void ensure_seq_cap(void)
{
    if (seq_size >= seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 32;
        seq_arr = (seq_t*)realloc(seq_arr, seq_cap * sizeof(seq_t));
        if (!seq_arr) exit(EXIT_FAILURE);
    }
}

idx_t _floor_isqrt_u64(idx_t n); /* prototype from utils, not used */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined indices treated as 0 */

    if (!primes_initialized) init_primes();

    /* fill the sequence up to required index */
    while (seq_size < (size_t)n) {
        ++last_checked;   /* examine next integer */

        /* test cubefree */
        bool cubefree = true;
        for (size_t pi = 0; pi < prime_cap; ++pi) {
            uint64_t p = prime_list[pi];
            uint64_t cube = p * p * p;
            if (cube > last_checked) break;
            if (last_checked % cube == 0) { cubefree = false; break; }
        }
        if (!cubefree) continue;   /* not a cubefree number */

        /* test squarefree (only if n>1, since 1 is special) */
        bool squarefree = true;
        if (last_checked > 1) {
            for (size_t pi = 0; pi < prime_cap; ++pi) {
                uint64_t p = prime_list[pi];
                uint64_t sq = p * p;
                if (sq > last_checked) break;
                if (last_checked % sq == 0) { squarefree = false; break; }
            }
        }

        /* compute a value for this cubefree number */
        seq_t val;
        if (last_checked == 1)          /* a(1) = 0 by definition */
            val = (seq_t)0;
        else if (squarefree)
            val = (seq_t)1;
        else
            val = (seq_t)2;

        ensure_seq_cap();
        seq_arr[seq_size++] = val;
    }

    return seq_arr[n - 1];
}
