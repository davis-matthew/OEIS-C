/*
 * A242218  Semiprimes which are the arithmetic mean of three consecutive
 *          primes.
 * https://oeis.org/A242218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic mean of three consecutive primes that is a
 *            semiprime. * Computed by pre‑computing all primes up to a fixed
 *            limit, determining the * arithmetic mean of each triple of
 *            consecutive primes that is divisible by 3, * testing whether
 *            that mean is a semiprime (exactly two prime factors...
 * Range:     u8 none, u16 n <= 305, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--------------------------------------------------------------*/
/* Standard library includes (C11 and C++17 compatible)         */
/*--------------------------------------------------------------*/
#include <stddef.h>   /* size_t, NULL */
#include <stdint.h>   /* uint64_t */
#include <stdbool.h>  /* bool */
#include <stdlib.h>   /* malloc, calloc, realloc, free */
#include "../utils/_smallest_prime_factor.h"

/*--------------------------------------------------------------*/
/* static state used for lazy initialization                  */
/*--------------------------------------------------------------*/
static bool                     initialized = false;
static uint64_t *                prime_list  = NULL;    /* primes up to LIMIT */
static size_t                   prime_count = 0;
static seq_t *                  answer_list = NULL;    /* a(1), a(2), ... */
static size_t                   answer_count = 0;

/*--------------------------------------------------------------*/
/* Build the primes up to 'limit' using the Sieve of Eratosthenes */
/*--------------------------------------------------------------*/
static void build_primes(idx_t limit)
{
    /* Allocate the composite flag array. Vectors in C++ must be cast. */
    bool *is_comp = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_comp) { free(prime_list); /* propagate failure */
                     return; }

    /* Allocate an initial buffer for the primes; it will grow if needed. */
    size_t cap = limit / 10 + 1000;   /* heuristic */
    prime_list = (uint64_t *)malloc(cap * sizeof(uint64_t));
    prime_count = 0;

    for (idx_t i = 2; i <= limit; ++i)
    {
        if (!is_comp[(size_t)i])
        {
            /* Ensure capacity */
            if (prime_count == cap)
            {
                cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, cap * sizeof(uint64_t));
                if (!tmp)
                {
                    free(is_comp);
                    free(prime_list);
                    return;
                }
                prime_list = tmp;
            }
            prime_list[prime_count++] = (uint64_t)i;

            /* Mark multiples */
            if (i * i <= limit)
            {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[(size_t)j] = true;
            }
        }
    }
    free(is_comp);
}

/*--------------------------------------------------------------*/
/* Collect all means that are semiprimes and store them in      */
/* 'answer_list'.  Only the first 'answer_count' entries are used */
/* to answer the sequence.                                      */
/*--------------------------------------------------------------*/
static void collect_answers(void)
{
    /* Rough upper bound: at most 'prime_count' answers. */
    answer_list = (seq_t *)malloc(prime_count * sizeof(seq_t));
    answer_count = 0;

    for (size_t i = 0; i + 2 < prime_count; ++i)
    {
        seq_t sum = (seq_t)prime_list[i] + (seq_t)prime_list[i + 1]
                  + (seq_t)prime_list[i + 2];
        if (sum % 3 != 0)
            continue;
        seq_t m = sum / 3;  /* arithmetic mean */

        /* Test whether m is a semiprime (exactly two prime factors counting
         * multiplicity).  Since m < ~4·10⁶, we can safely cast it to uint64_t. */
        uint64_t x = (uint64_t)m;
        int  factor_cnt = 0;
        while (x > 1)
        {
            uint64_t d = _smallest_prime_factor(x);
            while (x % d == 0)
            {
                x /= d;
                ++factor_cnt;
            }
        }
        if (factor_cnt == 2)
            answer_list[answer_count++] = m;
    }
}

/*--------------------------------------------------------------*/
/* Ensure that the static data structures are initialized.     */
/*--------------------------------------------------------------*/
static void ensure_init(void)
{
    if (initialized) return;

    /* 10 000th term well below the first 10⁵ primes – 10 million is more than
     * sufficient. */
    const idx_t SIEVE_LIMIT = 10000000ULL;
    build_primes(SIEVE_LIMIT);
    collect_answers();
    initialized = true;
}

/*--------------------------------------------------------------*/
/* Public sequence function                                    */
/*--------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* offset is 1; 0 is considered undefined */
    ensure_init();
    if (n > answer_count) return 0;   /* out of range */
    return answer_list[n - 1];
}
