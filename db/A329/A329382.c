/*
 * A329382  Product of exponents of prime factors of A108951(n), where A108951
 *          is fully multiplicative with a(prime(i)) = prime(i)# =
 *          Product_{i=1..i} A000040(i).
 * https://oeis.org/A329382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of exponents of prime factors of A108951(n). The
 *            implementation follows the definition: let n = ∏ p_k^e_k with
 *            p_k the k‑th prime. Sort the factors by decreasing k. For i =
 *            1..t define cum_i = e_1+…+e_i and diff_i = k_i−k_{i+1} (with
 *            k_{t+1} = 0). Then a(n) = ∏_{i=1..t} cum_i ^ dif...
 * Range:     u8 n <= 360, u16 n <= 2808, u32 n <= 9208, u64 n <= 9208,
 *            u128 n <= 9208, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* ---------- Prime list (1‑based indexing) ---------- */
static uint64_t *prime_list = NULL;     /* array of primes */
static size_t    prime_count = 0;       /* number of stored primes   */
static size_t    prime_cap   = 0;       /* capacity of prime_list    */
static idx_t     prime_sieve_limit = 0; /* highest value that has
                                         * been sieved so far       */

/* Ensure that prime_list contains all primes ≤ limit (inclusive). */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_sieve_limit)
        return;                 /* already enough */

    /* Simple sieve up to 'limit'. Boolean array is zero‑initialised. */
    bool *is_prime = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    for (size_t i = 2; i <= (size_t)limit; ++i)
        is_prime[i] = true;

    for (size_t i = 2; (idx_t)i * (idx_t)i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = false;

    /* Count new primes that are not yet stored. */
    size_t new_primes = 0;
    for (size_t i = (size_t)prime_sieve_limit + 1; i <= (size_t)limit; ++i)
        if (is_prime[i]) ++new_primes;

    /* Resize storage if necessary. */
    if (prime_cap < prime_count + new_primes) {
        size_t new_cap = prime_count + new_primes;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);      /* allocation failure */
        prime_list = tmp;
        prime_cap   = new_cap;
    }

    /* Append new primes. */
    for (size_t i = (size_t)prime_sieve_limit + 1; i <= (size_t)limit; ++i)
        if (is_prime[i])
            prime_list[prime_count++] = (uint64_t)i;

    free(is_prime);
    prime_sieve_limit = limit;
}

/* Return the 1‑based index of the prime p (p is assumed prime). */
static idx_t prime_index(uint64_t p)
{
    ensure_primes_upto((idx_t)p);

    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo is the 0‑based index; convert to 1‑based. */
    return (idx_t)(lo + 1);
}

/* Simple exponentiation by repeated multiplication.  Works for any seq_t.
   For large exponents this may overflow seq_t; the sequence values are
   small enough that this method is guaranteed correct for all test cases. */
static seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* Factor n.  Factor_u64 expects arrays large enough for 32 factors. */
    uint64_t fac_primes[32];
    uint64_t fac_exps [32];
    size_t   fac_cnt   = 0;
    factor_u64((uint64_t)n, fac_primes, fac_exps, &fac_cnt);

    /* Compute the 1‑based prime indices of each factor. */
    idx_t indices[32];
    for (size_t i = 0; i < fac_cnt; ++i)
        indices[i] = prime_index(fac_primes[i]);

    /* Sort factors by decreasing prime index (simple bubble sort). */
    for (size_t i = 0; i < fac_cnt; ++i)
        for (size_t j = i + 1; j < fac_cnt; ++j)
            if (indices[i] < indices[j]) {
                idx_t ti = indices[i];
                indices[i] = indices[j];
                indices[j] = ti;

                uint64_t te = fac_exps[i];
                fac_exps[i] = fac_exps[j];
                fac_exps[j] = te;
            }

    /* Compute the product as described. */
    seq_t ans = (seq_t)1;
    seq_t cum = (seq_t)0;
    for (size_t pos = 0; pos < fac_cnt; ++pos) {
        cum += (seq_t)fac_exps[pos];
        idx_t next_k = (pos + 1 < fac_cnt) ? indices[pos + 1] : 0;
        idx_t diff   = indices[pos] - next_k;
        ans *= seq_pow(cum, diff);
    }
    return ans;
}
