/*
 * A347622  Number of partitions of n into at most 2 prime parts (counting 1
 *          as a prime).
 * https://oeis.org/A347622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into at most two prime parts *
 *            (with 1 treated as prime)
 * Range:     u8 n <= 969, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* integer square root (floor) for idx_t (uint64_t). */
static inline size_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return (size_t)r;
}

/* Prime table and list -------------------------------------------- */
static size_t prime_limit = 0;          /* current maximum index of is_prime_table */
static bool *is_prime_table = NULL;     /* indices 0 .. prime_limit */
static size_t *prime_list   = NULL;     /* primes (including 1) */
static size_t prime_count   = 0;
static size_t prime_list_cap = 0;

/* Ensure prime data up to 'limit'. */
static void extend_primes(size_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Resize flag table */
    bool *tmp = (bool*)realloc(is_prime_table, (limit + 1) * sizeof(bool));
    if (!tmp)
        exit(EXIT_FAILURE);
    is_prime_table = tmp;

    /* New entries start as prime candidates */
    for (size_t i = prime_limit + 1; i <= limit; ++i)
        is_prime_table[i] = true;

    /* Sieve composites */
    size_t sqrt_lim = isqrt_u64(limit);
    for (size_t i = 2; i <= sqrt_lim; ++i) {
        if (is_prime_table[i]) {
            size_t start = i * i;
            if (start < prime_limit + 1) {
                start = ((prime_limit + 1 + i - 1) / i) * i;
            }
            for (size_t j = start; j <= limit; j += i)
                is_prime_table[j] = false;
        }
    }

    /* 0 is not prime; 1 is considered prime */
    is_prime_table[0] = false;
    if (limit >= 1)
        is_prime_table[1] = true;

    /* Grow prime list if necessary */
    if (prime_list_cap == 0) {
        prime_list_cap = 16;
        prime_list = (size_t*)malloc(prime_list_cap * sizeof(size_t));
        if (!prime_list)
            exit(EXIT_FAILURE);
        prime_count = 0;
        /* Include 1 as prime */
        prime_list[prime_count++] = 1;
    }

    for (size_t i = prime_limit + 1; i <= limit; ++i) {
        if (is_prime_table[i]) {
            if (prime_count == prime_list_cap) {
                prime_list_cap *= 2;
                size_t *tmp2 = (size_t*)realloc(prime_list, prime_list_cap * sizeof(size_t));
                if (!tmp2)
                    exit(EXIT_FAILURE);
                prime_list = tmp2;
            }
            prime_list[prime_count++] = i;
        }
    }

    prime_limit = limit;
}

/* Sequence cache ----------------------------------------------- */
static seq_t *cache     = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Extend cache to include n if needed */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        cache = tmp;

        /* Ensure primes up to n are ready */
        extend_primes((size_t)n);

        for (idx_t k = cache_size; k <= n; ++k) {
            seq_t val;
            if (k == 0) {
                val = 1; /* empty partition */
            } else {
                val = 0;

                /* Single part: k itself is prime (or 1) */
                if ((k == 1) || is_prime_table[(size_t)k])
                    val += 1;

                /* Unordered pairs p <= q with p + q = k */
                seq_t half = (seq_t)(k >> 1);
                for (size_t i = 1; i < prime_count; ++i) {
                    size_t p = prime_list[i];
                    if (p > (size_t)half)
                        break;
                    if (is_prime_table[(size_t)(k - p)])
                        val += 1;
                }
            }
            cache[(size_t)k] = val;
        }
        cache_size = new_size;
    }
    return cache[(size_t)n];
}
