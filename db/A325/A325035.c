/*
 * A325035  Product of sums of the multisets of prime indices of each prime
 *          index of 2 * n + 1.
 * https://oeis.org/A325035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of sums of prime‑index multisets of each prime
 *            index of 2*n+1, with a(0)=1
 * Range:     u8 n <= 6845, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/*  Static tables for prime data, built only once and expanded as needed. */
static idx_t *prime_list  = NULL;   // ascending list of primes
static size_t  prime_cnt   = 0;     // number of primes
static idx_t *prime_index = NULL;   // prime_index[p] = 1‑based index of prime p, 0 if not prime
static seq_t *k_sum       = NULL;   // k_sum[k] = sum of prime indices of k  (k >= 1)
static idx_t   sieve_limit = 0;     // maximum N for which tables are valid

/*  Ensure the tables are built for all numbers up to LIMIT (inclusive).  */
static void ensure_primes_upto(idx_t LIMIT)
{
    if (LIMIT <= sieve_limit) return;   /* already built */

    /* Allocate temporary sieve. */
    bool *is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    if (LIMIT >= 0) is_prime[0] = false;
    if (LIMIT >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    /* Count primes. */
    size_t count = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++count;

    /* Free old tables. */
    free(prime_list);
    free(prime_index);
    free(k_sum);

    /* Allocate new tables. */
    prime_list  = (idx_t*)malloc(count * sizeof(idx_t));
    prime_index = (idx_t*)calloc(LIMIT + 1, sizeof(idx_t));  /* 0 means “not a prime” */
    k_sum       = (seq_t*)calloc(count + 1, sizeof(seq_t));  /* 1‑based */

    if (!prime_list || !prime_index || !k_sum) exit(EXIT_FAILURE);

    /* Fill tables. */
    size_t pos = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) {
            prime_list[pos] = i;
            prime_index[i] = pos + 1;  /* 1‑based index */
            ++pos;
        }
    prime_cnt   = count;
    sieve_limit = LIMIT;

    /* Precompute k_sum[k] for k = 1 .. prime_cnt. */
    for (idx_t k = 1; k <= prime_cnt; ++k) {
        seq_t sum = 0;
        idx_t t = k;
        for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= t; ++i) {
            idx_t p = prime_list[i];
            if (t % p == 0) {
                int e = 0;
                while (t % p == 0) { t /= p; ++e; }
                idx_t idxP = prime_index[p];
                sum += (seq_t)idxP * e;
            }
        }
        if (t > 1) {                    /* remaining prime factor */
            idx_t idxP = prime_index[t];
            sum += (seq_t)idxP;
        }
        k_sum[k] = sum;
    }

    free(is_prime);
}

/*  Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0) return 1;              /* 2*0+1 = 1 → empty product */

    idx_t m = 2 * n + 1;               /* odd number whose prime factors are considered */
    ensure_primes_upto(m);

    seq_t result = 1;
    idx_t tmp = m;

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        idx_t p = prime_list[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            idx_t k = prime_index[p];         /* prime index of p (1‑based) */
            seq_t s = k_sum[k];               /* sum of prime indices of k */
            for (int j = 0; j < e; ++j)
                result *= s;                  /* multiply s e times */
        }
    }

    if (tmp > 1) {                      /* remaining prime factor */
        idx_t k = prime_index[tmp];
        seq_t s = k_sum[k];
        result *= s;
    }

    return result;
}
