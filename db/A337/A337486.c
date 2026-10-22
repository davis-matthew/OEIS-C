/*
 * A337486  Numbers k such that b(k+1) divides b(k), where b() is Recamán's
 *          multiplicative sequence A008336.
 * https://oeis.org/A337486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_factors, subtract_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recamán's multiplicative sequence (A008336):
   a(1) = 1, and for m > 1, a(m) is the smallest integer that
   can be constructed from a multiset of primes that evolves as
   follows:
        – Initially the multiset is empty.
        – For each integer candidate starting at 2:
              * If all its prime factors are present in the multiset
                with required multiplicities, the number is *created*,
                those factors are removed from the multiset, and the
                number becomes the next term of the sequence.
              * If it cannot be created, the prime factors of the
                candidate are *added* to the multiset.
   This function returns the n‑th term of that sequence (offset 1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_factors.h"
#include "../utils/subtract_factors.h"

/* Maximum prime we will sieve.  This is comfortably
   larger than all primes needed for n up to about 10000. */
#define MAX_PRIME 1000000

static bool initialized = false;
static uint32_t *prime_list = NULL;      /* array of primes */
static size_t     prime_count = 0;       /* number of primes */
static int32_t    *prime_index = NULL;   /* prime -> index in prime_list */

/* Simple sieve of Eratosthenes */
static void init_primes(void)
{
    if (initialized)
        return;
    initialized = true;

    bool *is_prime = (bool *)calloc(MAX_PRIME + 1, sizeof(bool));
    if (!is_prime)
        exit(1);
    for (int i = 2; i <= MAX_PRIME; ++i)
        is_prime[i] = true;

    size_t sz = 0;
    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (is_prime[i]) {
            ++sz;
            if ((int64_t)i * i <= MAX_PRIME) {
                for (int j = i * i; j <= MAX_PRIME; j += i)
                    is_prime[j] = false;
            }
        }
    }

    prime_list = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!prime_list)
        exit(1);
    prime_count = sz;

    prime_index = (int32_t *)malloc((MAX_PRIME + 1) * sizeof(int32_t));
    if (!prime_index)
        exit(1);
    for (int i = 0; i <= MAX_PRIME; ++i)
        prime_index[i] = -1;

    size_t idx = 0;
    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (is_prime[i]) {
            prime_list[idx] = (uint32_t)i;
            prime_index[i] = (int32_t)idx;
            ++idx;
        }
    }
    free(is_prime);
}

/* Factor a number (seq_t type).  Stores prime indices (positive for primes
   <=MAX_PRIME, or -1 for primes >MAX_PRIME) and exponents.
   Returns the count of distinct prime factors in '*cnt'. */
static void factor_seq(seq_t n,
                       int32_t *idx_arr, int *exp_arr, int *cnt)
{
    seq_t tmp = n;
    size_t i = 0;
    *cnt = 0;
    while (i < prime_count) {
        uint32_t p = prime_list[i];
        seq_t p2 = (seq_t)p * (seq_t)p;
        if (p2 > tmp)
            break;
        if (tmp % p == 0) {
            int e = 0;
            while ((tmp % p) == 0) {
                tmp /= p;
                ++e;
            }
            idx_arr[*cnt] = (int32_t)i;   /* index of prime */
            exp_arr[*cnt] = e;
            ++(*cnt);
        }
        ++i;
    }
    if (tmp > 1) {          /* remaining prime factor > sqrt(original) */
        uint64_t rem = (uint64_t)tmp;
        if (rem > MAX_PRIME) {
            /* If it exceeds our precomputed primes, treat it as a new prime. */
            /* For the intended usage (n <= 10000) this never happens. */
            /* We allocate a single slot for this prime. */
            static int32_t last_runtime_idx = -1;
            if (last_runtime_idx == -1) {
                last_runtime_idx = (int32_t)prime_count;
                ++prime_count;
            }
            idx_arr[*cnt] = last_runtime_idx;
            exp_arr[*cnt] = 1;
            ++(*cnt);
        } else {
            int32_t pi = prime_index[rem];
            idx_arr[*cnt] = pi;
            exp_arr[*cnt] = 1;
            ++(*cnt);
        }
    }
}

static void add_factors(int *counts, int32_t *idx_arr, int *exp_arr, int cnt)
{
    for (int i = 0; i < cnt; ++i) {
        int32_t pi = idx_arr[i];
        int e = exp_arr[i];
        counts[pi] += e;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;      /* undefined, but never used */
    if (n == 1)
        return (seq_t)1;

    init_primes();

    /* counts of each prime in the multiset */
    int *counts = (int *)calloc(prime_count, sizeof(int));
    if (!counts)
        exit(1);

    int found = 1;   /* we already have the first term 1 */
    seq_t candidate = 2;   /* next integer to check */

    /* temporary storage for factorization (max 32 distinct primes for our range) */
    int32_t fac_idx[32];
    int    fac_exp[32];
    int    fac_cnt;

    while (found < n) {
        factor_seq(candidate, fac_idx, fac_exp, &fac_cnt);

        if (has_factors(counts, fac_idx, fac_exp, fac_cnt)) {
            /* create the number */
            subtract_factors(counts, fac_idx, fac_exp, fac_cnt);
            ++found;
            if (found == n) {
                free(counts);
                return candidate;
            }
        } else {
            /* add the number to the multiset */
            add_factors(counts, fac_idx, fac_exp, fac_cnt);
        }
        ++candidate;
    }

    free(counts);   /* should not be reached for n>=1 */
    return candidate;
}
