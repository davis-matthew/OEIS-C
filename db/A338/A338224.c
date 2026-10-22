/*
 * A338224  Multiplicative with a(A246655(k)) = prime(k) for any k > 0 (where
 *          prime(k) denotes the k-th prime number).
 * https://oeis.org/A338224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   multiplicative function with a(A246655(k)) = prime(k) where
 *            A246655(k) is the k‑th positive integer that is a power of a
 *            prime (including the primes themselves). To evaluate a(n),
 *            factor n into distinct prime powers pᵉ and set a(n) = ∏
 *            prime(rank(pᵉ)), where rank(pᵉ) is the position of pᵉ among a...
 * Range:     u8 n <= 94, u16 n <= 7919, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*---------------------  Prime table  ---------------------*/
static idx_t  *PRIME_LIST   = NULL;   /* primes in ascending order */
static size_t   PRIME_COUNT = 0;

/*---------------------  Rank table  ---------------------*/
static idx_t *POWER_RANK      = NULL;   /* POWER_RANK[v] = rank of v if v is a prime power, else 0 */
static idx_t  POWER_RANK_LIMIT = 0;

/* Comparator for qsort (ascending) */
static int cmp_uint64(const void *a, const void *b)
{
    const idx_t va = *(const idx_t *)a;
    const idx_t vb = *(const idx_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

/* Build prime table up to a fixed large limit (2,000,000).  Called once. */
static void init_primes(void)
{
    if (PRIME_LIST != NULL)
        return;                     /* already initialized */

    const idx_t SIEVE_LIMIT = 2000000;
    uint8_t *is_prime = (uint8_t *)malloc(SIEVE_LIMIT + 1);
    if (!is_prime)
        return;
    memset(is_prime, 1, SIEVE_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* store primes */
    size_t capacity = 32768;
    PRIME_LIST = (idx_t *)malloc(capacity * sizeof(idx_t));
    if (!PRIME_LIST) {
        free(is_prime);
        return;
    }
    PRIME_COUNT = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            if (PRIME_COUNT == capacity) {
                capacity <<= 1;
                idx_t *tmp = (idx_t *)realloc(PRIME_LIST, capacity * sizeof(idx_t));
                if (!tmp) {
                    free(PRIME_LIST);
                    PRIME_LIST = NULL;
                    free(is_prime);
                    return;
                }
                PRIME_LIST = tmp;
            }
            PRIME_LIST[PRIME_COUNT++] = i;
        }
    }
    free(is_prime);
}

/* Ensure POWER_RANK contains ranks for all prime powers ≤ limit. */
static void ensure_power_rank(idx_t limit)
{
    if (limit <= POWER_RANK_LIMIT)
        return;     /* already up to date */

    /* Allocate new array (zeros). */
    idx_t *new_rank = (idx_t *)calloc(limit + 1, sizeof(idx_t));
    if (!new_rank)
        return;

    /* Gather all prime powers ≤ limit. */
    size_t pp_capacity = 128;
    idx_t *prime_powers = (idx_t *)malloc(pp_capacity * sizeof(idx_t));
    size_t pp_count = 0;

    for (size_t i = 0; i < PRIME_COUNT && PRIME_LIST[i] <= limit; ++i) {
        idx_t p = PRIME_LIST[i];
        idx_t val = p;
        while (val <= limit) {
            if (pp_count == pp_capacity) {
                pp_capacity <<= 1;
                idx_t *tmp = (idx_t *)realloc(prime_powers, pp_capacity * sizeof(idx_t));
                if (!tmp) {
                    free(prime_powers);
                    free(new_rank);
                    return;
                }
                prime_powers = tmp;
            }
            prime_powers[pp_count++] = val;
            if (val > limit / p)
                break;
            val *= p;
        }
    }

    /* Sort the powers to establish order. */
    qsort(prime_powers, pp_count, sizeof(idx_t), cmp_uint64);

    /* Assign ranks (1‑based). */
    for (size_t i = 0; i < pp_count; ++i)
        new_rank[prime_powers[i]] = (idx_t)(i + 1);

    /* Replace global array. */
    free(POWER_RANK);
    POWER_RANK = new_rank;
    POWER_RANK_LIMIT = limit;
    free(prime_powers);
}

/*---------------------  Sequence function  ---------------------*/
seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    init_primes();                 /* ensure prime table is ready */
    ensure_power_rank(n);          /* build rank table up to n */

    seq_t  result = (seq_t)1;
    idx_t  temp   = n;

    /* Factor n using primes up to sqrt(temp). */
    for (size_t i = 0; i < PRIME_COUNT; ++i) {
        idx_t p = PRIME_LIST[i];
        if (p * p > temp)
            break;
        if (temp % p == 0) {
            idx_t power_val = 1;   /* p^e */
            while (temp % p == 0) {
                temp   /= p;
                power_val *= p;
            }
            idx_t rank = POWER_RANK[power_val];
            seq_t prime_val = (seq_t)PRIME_LIST[rank - 1];
            result *= prime_val;
        }
    }

    /* Remaining prime factor (if any). */
    if (temp > (idx_t)1) {
        idx_t rank = POWER_RANK[temp];
        seq_t prime_val = (seq_t)PRIME_LIST[rank - 1];
        result *= prime_val;
    }

    return result;
}
