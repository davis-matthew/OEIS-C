/*
 * A375384  Triangular numbers that are sandwiched between two squarefree
 *          semiprimes.
 * https://oeis.org/A375384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 17, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th triangular number that is sandwiched between two
 * squarefree semiprimes. That is, for a triangular number T = k(k+1)/2
 * we require that (T-1) and (T+1) are both products of two distinct primes.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define PRIME_SIEVE_LIMIT 5000000  /* enough for k up to about 2e6 */

static bool sieve_done = false;
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap = 0;

/*------------------------------------------------------------*/
/*  Simple prime sieve – fills prime_list with all primes ≤ 5e6   */
/*------------------------------------------------------------*/
static void init_prime_sieve(void)
{
    if (sieve_done) return;
    sieve_done = true;

    bool *is_comp = (bool*)malloc((PRIME_SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_comp) exit(1);
    memset(is_comp, 0, (PRIME_SIEVE_LIMIT + 1));
    is_comp[0] = is_comp[1] = true;

    for (uint32_t i = 2; i * (uint32_t)i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i) {
                is_comp[j] = true;
            }
        }
    }

    /* estimate number of primes for allocation */
    prime_cap = PRIME_SIEVE_LIMIT / 10;  /* rough */
    prime_list = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_list) exit(1);

    prime_count = 0;
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                uint32_t *tmp = (uint32_t*)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                prime_list = tmp;
            }
            prime_list[prime_count++] = i;
        }
    }
    free(is_comp);
}

/*------------------------------------------------------------*/
/*  64‑bit deterministic Miller–Rabin isPrime                 */
/*  (Simplified using trial division with precomputed primes) */
/*------------------------------------------------------------*/
static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    init_prime_sieve();
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return n == p;
    }
    return true;
}

/*------------------------------------------------------------*/
/*  Check if n is a squarefree semiprime (product of two
 *  distinct primes).                                       */
/*------------------------------------------------------------*/
static bool is_squarefree_semiprime(uint64_t n)
{
    if (n < 6) return false;  /* smallest such is 6 = 2*3 */

    if (is_prime_64(n)) return false;

    /* Find smallest prime factor */
    uint64_t f = 0;
    init_prime_sieve();
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) {
            f = p;
            break;
        }
    }
    if (f == 0) return false;           /* didn't find factor, so n is prime */
    if (n % f == 0 && n / f == f) return false; /* square factor -> not squarefree */

    uint64_t rem = n / f;
    if (rem == f) return false;          /* equal factors, not distinct */
    if (!is_prime_64(rem)) return false;  /* more than two primes */

    return true;
}

/*------------------------------------------------------------*/
/*  Cache for computed sequence values                        */
/*------------------------------------------------------------*/
static seq_t *seq_cache = NULL;
static size_t seq_cache_used = 0;
static size_t seq_cache_cap  = 0;

/* grow cache to at least 'need' slots, doubling strategy */
static void grow_cache(size_t need)
{
    if (seq_cache_cap >= need) return;
    size_t new_cap = seq_cache_cap > 0 ? seq_cache_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_cache = tmp;
    seq_cache_cap = new_cap;
}

/*------------------------------------------------------------*/
/*  The required function                                    */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* sequence is defined only for n≥1 */

    /* Ensure cache contains at least n terms */
    while (seq_cache_used < n) {
        /* Generate next triangular number */
        static uint64_t next_k = 1;
        uint64_t k = next_k++;
        __int128 tri = (__int128)k * (k + 1) / 2;
        uint64_t T = (uint64_t)tri;  /* fits in 64 bits for sought n */

        if (is_squarefree_semiprime(T - 1) &&
            is_squarefree_semiprime(T + 1)) {
            grow_cache(seq_cache_used + 1);
            seq_cache[seq_cache_used++] = (seq_t)T;
        }
    }

    return seq_cache[n - 1];
}
