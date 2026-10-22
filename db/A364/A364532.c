/*
 * A364532  Positive integers with a prime index equal to the sum of prime
 *          indices of some nonprime divisor. Heinz numbers of a variation of
 *          sum-full partitions.
 * https://oeis.org/A364532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A364532: Positive integers with a prime index equal to the sum of
    prime indices of some nonprime divisor.  The function scans
    integers sequentially, testing the property by factorising each
    candidate and examining all composite divisors.  The results are
    memoised so that any call to a(idx_t n) is answered in O(1) after
    the first n terms have been built.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_pi.h"

/* ------------------------------------------------------------------ */
/* Prime sieve (up to 1 000 000) for factorisation and prime index lookup. */
/* ------------------------------------------------------------------ */
static const uint64_t SIEVE_LIMIT = 1000000ULL;
static bool *is_prime_arr = NULL;
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static bool sieve_built = false;

/* Build a simple sieve of Eratosthenes up to SIEVE_LIMIT. */
static void build_sieve(void)
{
    if (sieve_built) return;
    size_t n = SIEVE_LIMIT;
    is_prime_arr = (bool*)malloc((n + 1) * sizeof(bool));
    for (size_t i = 0; i <= n; ++i) is_prime_arr[i] = true;
    is_prime_arr[0] = is_prime_arr[1] = false;
    for (size_t i = 2; i * i <= n; ++i) {
        if (is_prime_arr[i]) {
            for (size_t j = i * i; j <= n; j += i)
                is_prime_arr[j] = false;
        }
    }
    prime_count = 0;
    for (size_t i = 2; i <= n; ++i)
        if (is_prime_arr[i]) ++prime_count;
    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= n; ++i)
        if (is_prime_arr[i]) prime_list[idx++] = (uint32_t)i;
    sieve_built = true;
}

/* Binary search for a prime within the pre‑computed sieve array. */
static inline uint32_t binary_search_prime(uint32_t p)
{
    size_t lo = 0, hi = prime_count;      /* hi is exclusive */
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (uint32_t)(lo + 1);             /* 1‑based index */
}

/* Return the index of a prime p.  p is guaranteed to be prime. */
static inline uint64_t prime_index(uint64_t p)
{
    if (p <= SIEVE_LIMIT) {
        return binary_search_prime((uint32_t)p);
    } else {
        seq_t pi = compute_pi((idx_t)p);
        return (uint64_t)pi;
    }
}

/* ------------------------------------------------------------------ */
/* Factorisation of a 64‑bit integer using the pre‑computed prime list. */
/* ------------------------------------------------------------------ */
static void factor_uint64(uint64_t n, uint64_t *pr, int *exps, size_t *cnt)
{
    size_t c = 0;
    uint64_t rem = n;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= rem; ++i) {
        uint64_t p = prime_list[i];
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            pr[c] = p;
            exps[c] = e;
            ++c;
        }
    }
    if (rem > 1) {
        pr[c] = rem;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* ------------------------------------------------------------------ */
/* Recursive enumeration of all divisor exponent combinations. */
/* Returns true if a composite divisor with required property is found.*/
/* ------------------------------------------------------------------ */
static bool dfs(int pos, uint64_t sum, int primes_used, bool multiexp,
                const uint64_t *indices, const int *exps, size_t k,
                const uint64_t *uniq, size_t uniq_sz, bool *found)
{
    if (*found) return true;
    if (pos == (int)k) {
        if (primes_used == 0) return false;                 /* divisor 1 */
        if (primes_used == 1 && !multiexp) return false;    /* prime divisor */
        /* composite divisor -> check if its sum matches any prime index
           of the original number */
        for (size_t i = 0; i < uniq_sz; ++i)
            if (sum == uniq[i]) { *found = true; return true; }
        return false;
    }
    for (int e = 0; e <= exps[pos]; ++e) {
        uint64_t new_sum = sum + (uint64_t)e * indices[pos];
        bool new_mult = multiexp || (e > 1);
        int new_used = primes_used + (e > 0);
        if (dfs(pos + 1, new_sum, new_used, new_mult,
                indices, exps, k, uniq, uniq_sz, found))
            return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Test whether a given number satisfies the sequence property. */
/* ------------------------------------------------------------------ */
static bool satisfies(seq_t val)
{
    uint64_t x = (uint64_t)val;
    uint64_t primes[64];
    int exps[64];
    size_t cnt;
    factor_uint64(x, primes, exps, &cnt);

    uint64_t indices[64];
    for (size_t i = 0; i < cnt; ++i)
        indices[i] = prime_index(primes[i]);

    /* unique prime indices of n */
    uint64_t uniq[64];
    size_t uniq_sz = 0;
    for (size_t i = 0; i < cnt; ++i) {
        bool exists = false;
        for (size_t j = 0; j < uniq_sz; ++j) {
            if (uniq[j] == indices[i]) { exists = true; break; }
        }
        if (!exists) uniq[uniq_sz++] = indices[i];
    }

    bool found = false;
    dfs(0, 0, 0, false, indices, exps, cnt, uniq, uniq_sz, &found);
    return found;
}

/* ------------------------------------------------------------------ */
/* Main function required by the evaluation framework.                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static memoised sequence */
    static seq_t *sequence = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static uint64_t candidate = 2;   /* start scanning from 2 */

    if (!sieve_built) build_sieve();

    while (seq_len < (size_t)n) {
        if (candidate == 0) return 0; /* overflow guard */

        if (satisfies((seq_t)candidate)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                sequence = (seq_t*)realloc(sequence, seq_cap * sizeof(seq_t));
            }
            sequence[seq_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
    return sequence[n - 1];
}
