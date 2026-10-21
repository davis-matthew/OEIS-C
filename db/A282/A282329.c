/*
 * A282329  Start with 2, then successively subtract the primes 3, 5, 7, ...
 * https://oeis.org/A282329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A007504(n+1) - 2*A071148(n-1)
 * Range:     i8 n <= 10, i16 n <= 39, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: a(n) = 4 - sum_{i=1}^n p_i, where p_i is the i‑th prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static storage for primes and their prefix sums. */
static uint64_t *prime_list   = NULL;  /* prime_list[0..prime_cnt-1] */
static seq_t   *prime_sum    = NULL;  /* prime_sum[i] = sum of first i primes, i>=0 */
static size_t  prime_cap    = 0;      /* allocated capacity for prime_list */
static size_t  prime_cnt    = 0;      /* number of primes stored */
static uint64_t next_candidate = 3;   /* next odd to test for primality */

/* Adjust storage size when needed. */
static void grow_prime_storage(void)
{
    if (prime_cnt < prime_cap)
        return;

    size_t new_cap = prime_cap ? prime_cap * 2 : 8;

    uint64_t *new_list =
        (uint64_t *)realloc((void *)prime_list, new_cap * sizeof(uint64_t));
    if (!new_list)
        abort();
    prime_list = new_list;

    seq_t *new_sum =
        (seq_t *)realloc((void *)prime_sum, (new_cap + 1) * sizeof(seq_t));
    if (!new_sum)
        abort();
    prime_sum = new_sum;

    prime_cap = new_cap;
}

/* Add a new prime to the table and update the cumulative sum. */
static void add_prime(uint64_t p)
{
    grow_prime_storage();

    prime_list[prime_cnt] = p;
    /* prefix sum: sum of first prime_cnt+1 primes */
    prime_sum[prime_cnt + 1] = prime_sum[prime_cnt] + (seq_t)p;
    prime_cnt++;
}

/* Initialise the tables with the first prime 2. */
static void init_primes(void)
{
    if (prime_list)
        return;                        /* already initialised */

    prime_cap = 8;
    prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    if (!prime_list)
        abort();

    prime_sum = (seq_t *)malloc((prime_cap + 1) * sizeof(seq_t));
    if (!prime_sum)
        abort();

    prime_cnt = 0;
    prime_sum[0] = (seq_t)0;

    add_prime(2);                       /* first prime */
    next_candidate = 3;                 /* next odd to test */
}

/* Ensure that we have at least 'need' primes in the cache. */
static void ensure_primes(idx_t need)
{
    init_primes();

    while (prime_cnt < (size_t)need) {
        uint64_t cand = next_candidate;
        next_candidate += 2;           /* only odd numbers remain */

        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            /* stop when p*p > cand; use division to avoid overflow */
            if (p > cand / p)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            add_prime(cand);
    }
}

/* --------------------------------------------------------------- */
/* Sequence function                                           */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* harmless for out‑of‑range */

    ensure_primes(n);                  /* make sure we have n primes */
    return (seq_t)4 - prime_sum[(size_t)n];
}
