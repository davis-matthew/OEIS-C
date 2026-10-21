/*
 * A257691  Numbers that are not A000120-abundant.
 * https://oeis.org/A257691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 9820, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A257691: 
   The n-th integer whose sum of binary popcounts of proper divisors
   does not exceed its own binary popcount. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* popcount for idx_t (always uint64_t) */
static inline int popcount_idx(idx_t x)
{
    return __builtin_popcountll((unsigned long long)x);
}

/* static storage for generated terms */
static idx_t *seq_arr = NULL;        /* array of qualifying numbers */
static size_t seq_size = 0;          /* number of terms stored */

/* temporary storage for sums of proper-divisor popcounts */
static uint64_t *sum_pop = NULL;
static idx_t sieve_limit = 0;        /* maximum n for which sum_pop is computed */

/* --------------------------------------------------------------------- */
/* Recalculate all terms up to and including `limit`                  */
/* --------------------------------------------------------------------- */
static void compute_up_to(idx_t limit)
{
    if (limit <= sieve_limit)
        return;                       /* nothing to do */

    /* allocate sum_pop array */
    sum_pop = (uint64_t *)realloc(sum_pop, (limit + 1) * sizeof(uint64_t));
    if (!sum_pop) abort();
    memset(sum_pop, 0, (limit + 1) * sizeof(uint64_t));

    /* accumulate popcount of proper divisors */
    for (idx_t d = 1; d <= limit; ++d) {
        int pc = popcount_idx(d);
        for (idx_t m = d << 1; m <= limit; m += d) {
            sum_pop[m] += (uint64_t)pc;
        }
    }

    /* rebuild seq_arr */
    seq_arr = (idx_t *)realloc(seq_arr, limit * sizeof(idx_t));
    if (!seq_arr) abort();
    seq_size = 0;
    for (idx_t n = 1; n <= limit; ++n) {
        if (sum_pop[n] <= (uint64_t)popcount_idx(n)) {
            seq_arr[seq_size++] = n;
        }
    }

    sieve_limit = limit;
}

/* --------------------------------------------------------------------- */
/* Make sure `need_terms` terms are available                           */
/* --------------------------------------------------------------------- */
static void ensure_terms(idx_t need_terms)
{
    if (seq_size >= need_terms)
        return;

    idx_t limit = (sieve_limit > 0) ? sieve_limit : 1000;
    while (seq_size < need_terms) {
        limit <<= 1;                 /* double the limit */
        compute_up_to(limit);
    }
}

/* --------------------------------------------------------------------- */
/* Public function required by the framework                           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined behaviour for n=0 */

    /* initialise once */
    static bool inited = false;
    if (!inited) {
        compute_up_to(200000);      /* covers the first ~10 000 terms easily */
        inited = true;
    }

    if (n > seq_size) {
        ensure_terms(n);            /* generate more if necessary */
    }

    return (seq_t)seq_arr[n - 1];
}
