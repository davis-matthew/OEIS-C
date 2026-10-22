/*
 * A350849  Number of odd conjugate parts minus number of even parts in the
 *          integer partition with Heinz number n.
 * https://oeis.org/A350849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (# odd parts in the conjugate of the partition
 *            corresponding to n's Heinz number) – (# even parts in that
 *            partition)
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Utility: prime generation ---------- */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

static void generate_primes_upto(idx_t limit, uint64_t **out, size_t *cnt)
{
    /* Simple sieve of Eratosthenes. */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) {
        *out = NULL;
        *cnt = 0;
        return;
    }
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= (uint64_t)limit; j += i)
                is_prime[(size_t)j] = false;
        }
    }

    /* Count primes. */
    size_t total = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++total;

    *out = (uint64_t *)malloc(total * sizeof(uint64_t));
    *cnt = total;
    if (*out == NULL) {
        free(is_prime);
        *cnt = 0;
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) (*out)[idx++] = (uint64_t)i;

    free(is_prime);
}

static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;
    free(prime_list);
    generate_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Find 1‑based index of prime p in prime_list. Returns 0 if not found. */
static idx_t prime_index_of(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p) lo = mid + 1;
        else if (prime_list[mid] > p) hi = mid;
        else return (idx_t)(mid + 1);   /* 1‑based */
    }
    return 0;   /* not a prime (should not happen for valid inputs) */
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* n == 0 is outside the intended range; return 0. */
    if (n == 0) return (seq_t)0;

    /* Ensure we have primes up to n for factorisation and indexing. */
    ensure_primes_upto(n);

    /* Factor n and collect (index, exponent) pairs. */
    uint64_t indices[64];
    uint64_t exps[64];
    size_t distinct = 0;
    uint64_t remaining = (uint64_t)n;
    idx_t max_idx = 0;

    for (size_t i = 0; i < prime_count && prime_list[i] <= remaining / (uint64_t)prime_list[i]; ++i) {
        uint64_t p = prime_list[i];
        if (remaining % p == 0) {
            uint64_t e = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++e;
            }
            idx_t idxp = prime_index_of(p);
            indices[distinct] = (uint64_t)idxp;
            exps[distinct] = e;
            if (idxp > max_idx) max_idx = idxp;
            ++distinct;
        }
    }
    if (remaining > 1) { /* remaining is a prime > sqrt(original n) */
        idx_t idxp = prime_index_of(remaining);
        indices[distinct] = (uint64_t)idxp;
        exps[distinct] = 1;
        if (idxp > max_idx) max_idx = idxp;
        ++distinct;
    }

    /* Count even parts directly. */
    int even_parts = 0;
    for (size_t i = 0; i < distinct; ++i)
        if ((indices[i] & 1ULL) == 0)
            even_parts += (int)exps[i];

    /* Build frequency array for odd-conjugate calculation. */
    int *cnt = (int *)calloc((size_t)max_idx + 2, sizeof(int));
    if (!cnt) return (seq_t)(-1); /* allocation failure */

    for (size_t i = 0; i < distinct; ++i)
        cnt[(size_t)indices[i]] += (int)exps[i];

    /* Count odd conjugate parts. */
    int odd_conj = 0;
    int cumulative = 0;
    for (int j = (int)max_idx; j >= 1; --j) {
        cumulative += cnt[j];
        if (cumulative & 1) ++odd_conj;
    }

    free(cnt);

    seq_t result = (seq_t)(odd_conj - even_parts);
    return result;
}
