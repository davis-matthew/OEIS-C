/*
 * A338093  Composite numbers which are multiples of the sum of the squares of
 *          their prime factors (taken with multiplicity).
 * https://oeis.org/A338093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 80, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Compute OEIS sequence A338093.
   The function builds a sieve of smallest prime factors, then collects
   composite numbers that are divisible by the sum of the squares of 
   their prime factors (with multiplicity).  The collected numbers are
   stored in a dynamic array and returned in order.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Globals for caching */
static idx_t    sequence_limit  = 0;          /* maximum n already sieved */
static uint32_t *spf_array      = NULL;       /* smallest prime factor table */
static unsigned long long *sum_sq_array = NULL; /* sum of squares of prime factors */
static unsigned long long *seq_values_big = NULL; /* collected terms (big type) */
static size_t   seq_len   = 0;                /* number of terms currently stored */
static size_t   seq_cap   = 0;                /* current allocation of seq_values_big */

/* Build or extend all tables up to new_limit.  Previously allocated tables
   are freed and rebuilt. */
static void build_tables(idx_t new_limit)
{
    if (sequence_limit >= new_limit)
        return;

    /* Free old tables if they exist */
    free(spf_array);
    free(sum_sq_array);
    free(seq_values_big);
    seq_values_big = NULL;
    seq_len = 0;
    seq_cap = 0;

    sequence_limit = new_limit;

    /* Allocate arrays */
    size_t lim = (size_t)new_limit;
    spf_array      = (uint32_t *)calloc(lim + 1, sizeof(uint32_t));
    sum_sq_array   = (unsigned long long *)calloc(lim + 1, sizeof(unsigned long long));
    if (!spf_array || !sum_sq_array)
        exit(EXIT_FAILURE); /* allocation failed */

    /* Linear sieve for smallest prime factor */
    size_t prime_cap = (size_t)new_limit / 10 + 16;
    uint32_t *primes = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    size_t prime_cnt = 0;

    for (idx_t i = 2; i <= new_limit; ++i)
    {
        if (!spf_array[(size_t)i])
        {
            spf_array[(size_t)i] = (uint32_t)i;
            if (prime_cnt >= prime_cap)
            {
                prime_cap <<= 1;
                primes = (uint32_t *)realloc(primes, prime_cap * sizeof(uint32_t));
            }
            primes[prime_cnt++] = (uint32_t)i;
        }
        uint32_t pi;
        for (size_t j = 0; j < prime_cnt; ++j)
        {
            pi = primes[j];
            if (pi > spf_array[(size_t)i])
                break;
            idx_t prod = i * (idx_t)pi;
            if (prod > new_limit)
                break;
            spf_array[(size_t)prod] = pi;
        }
    }
    free(primes);

    /* Sum of squares of prime factors with multiplicity */
    sum_sq_array[1] = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
    {
        uint32_t p = spf_array[(size_t)i];
        sum_sq_array[(size_t)i] = sum_sq_array[(size_t)(i / p)] +
                                  (unsigned long long)p * (unsigned long long)p;
    }

    /* Collect sequence terms */
    for (idx_t i = 2; i <= new_limit; ++i)
    {
        if (spf_array[(size_t)i] != (uint32_t)i)   /* composite   */
        {
            unsigned long long s = sum_sq_array[(size_t)i];
            if (s != 0 && ((unsigned long long)i % s) == 0)
            {
                if (seq_len == seq_cap)
                {
                    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
                    seq_values_big = (unsigned long long *)realloc(
                        seq_values_big, new_cap * sizeof(unsigned long long));
                    seq_cap = new_cap;
                }
                seq_values_big[seq_len++] = (unsigned long long)i;
            }
        }
    }
}

/* Ensure at least 'required' terms have been computed. */
static void ensure_terms(idx_t required)
{
    if (seq_len >= (size_t)required)
        return;

    idx_t new_limit = sequence_limit ? sequence_limit * 2 : 1000000;
    while (seq_len < (size_t)required)
    {
        build_tables(new_limit);
        if (seq_len >= (size_t)required)
            break;
        new_limit *= 2;
    }
}

/* Public function: a(n).  idx_t n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined; return 0 as placeholder */

    ensure_terms(n);
    return (seq_t)seq_values_big[n - 1];
}
