/*
 * A376596  Second differences of consecutive prime-powers inclusive
 *          (A000961). First differences of A057820.
 * https://oeis.org/A376596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = B[n+2] - 2*B[n+1] + B[n] where B is the prime‑powers
 *            inclusive sequence (A000961), 1-indexed. The implementation
 *            builds B on demand and uses it to compute the second differences
 *            of its first differences (A057820)
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto */

static idx_t *prime_powers = NULL;   /* prime powers, 0‑based; prime_powers[0] = 1   */
static size_t prime_pow_size = 0;    /* current number of stored elements          */
static idx_t sieve_limit = 0;        /* current upper bound of the sieve          */

/* Comparison function for qsort (unsigned 64‑bit integers) */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t av = *(const uint64_t *)a;
    uint64_t bv = *(const uint64_t *)b;
    return (av < bv) ? -1 : (av > bv ? 1 : 0);
}

/* Ensure that prime_powers contains at least 'need' elements (1‑based indexing). */
static void ensure_prime_powers(size_t needed)
{
    /* If we already have enough, nothing to do. */
    if (prime_pow_size >= needed)
        return;

    /* Start with a reasonable limit based on the last sieve. */
    idx_t limit = (sieve_limit > 2) ? sieve_limit : 2;

    while (1) {
        /* Step 1: generate all primes up to 'limit'. */
        uint64_t *primes = NULL;
        size_t pr_cnt = 0;
        build_primes_upto(limit, &primes, &pr_cnt);
        if (!primes) /* unlikely: out of memory */ return;

        /* Step 2: count how many prime powers are <= limit. */
        size_t count_est = 1;   /* count the value 1 */
        for (size_t i = 0; i < pr_cnt; ++i) {
            uint64_t p = primes[i];
            uint64_t v = p;
            while (v <= (uint64_t)limit) {
                ++count_est;
                if (v > (uint64_t)limit / p)
                    break;
                v *= p;
            }
        }

        /* If not enough, increase the limit and retry. */
        if (count_est < needed) {
            free(primes);
            limit <<= 1; /* double */
            continue;
        }

        /* Step 3: actually collect the prime powers. */
        idx_t *new_arr = (idx_t *)malloc(count_est * sizeof(idx_t));
        if (!new_arr) {
            /* Allocation failure: keep old data (if any). */
            free(primes);
            return;
        }

        size_t idx = 0;
        new_arr[idx++] = 1;                /* include the value 1 */

        for (size_t i = 0; i < pr_cnt; ++i) {
            uint64_t p = primes[i];
            uint64_t v = p;
            while (v <= (uint64_t)limit) {
                new_arr[idx++] = (idx_t)v;
                if (v > (uint64_t)limit / p)
                    break;
                v *= p;
            }
        }

        /* Sanity check: idx should equal count_est. */
        /* Step 4: sort the array (duplicates are impossible here). */
        qsort(new_arr, count_est, sizeof(idx_t), cmp_uint64);

        /* Step 5: replace old data. */
        free(prime_powers);
        prime_powers = new_arr;
        prime_pow_size = count_est;
        sieve_limit = limit;

        free(primes);
        break; /* finished */
    }
}

/*---  OEIS sequence implementation  ------------------------------------*/

seq_t a(idx_t n)
{
    /* We need prime_powers[n-1], prime_powers[n], prime_powers[n+1]  */
    /* Ensure the array has at least n+2 elements (1‑based).          */
    ensure_prime_powers((size_t)n + 2);

    seq_t bm1 = (seq_t)prime_powers[n - 1]; /* B[n]  */
    seq_t b   = (seq_t)prime_powers[n];     /* B[n+1]  */
    seq_t bp1 = (seq_t)prime_powers[n + 1]; /* B[n+2]  */

    return bp1 - (b * 2) + bm1;
}
