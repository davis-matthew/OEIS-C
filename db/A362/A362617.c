/*
 * A362617  Numbers whose prime factorization has both (1) even length, and
 *          (2) unequal middle parts.
 * https://oeis.org/A362617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A362617: numbers whose prime factorisation has
 * (1) even total multiplicity and (2) distinct middle two factors.
 * The function computes the n‑th element by incremental search,
 * caching results for future calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- Static cache for the sequence values --- */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;   /* number of terms stored      */
static size_t seq_cap  = 0;   /* current array capacity      */

/* --- Static cache for generated primes --- */
static uint64_t *primes          = NULL;
static size_t    primes_len      = 0;
static size_t    primes_cap      = 0;
static uint64_t  primes_last_chk = 1;   /* last integer tested for primality */

/* --- Last integer that was checked during the search --- */
static uint64_t last_tested = 1;        /* 1 → first candidate will be 2 */

/* ------------------------------------------------------------------ */
/* Ensure the global prime array contains all primes ≤ limit.         */
static void
ensure_primes_up_to(uint64_t limit)
{
    while (primes_last_chk < limit) {
        ++primes_last_chk;
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > primes_last_chk) break;
            if (primes_last_chk % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_cap == 0) {
                primes_cap = 256;
                primes     = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
                if (!primes) exit(EXIT_FAILURE);
            } else if (primes_len == primes_cap) {
                primes_cap <<= 1;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!primes) exit(EXIT_FAILURE);
            }
            primes[primes_len++] = primes_last_chk;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Append a newly found sequence term, enlarging the cache if needed. */
static void
seq_push(seq_t val)
{
    if (seq_cap == 0) {
        seq_cap = 1024;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache) exit(EXIT_FAILURE);
    }
    if (seq_len == seq_cap) {
        seq_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
        if (!seq_cache) exit(EXIT_FAILURE);
    }
    seq_cache[seq_len++] = val;
}

/* ------------------------------------------------------------------ */
/* Test whether a number satisfies the sequence condition.           */
/* The function uses trial division with the global prime array.     */
static bool
good_factorization(uint64_t n)
{
    /* Factorization lists */
    uint64_t prime_list[128];
    size_t   exp_list[128];
    size_t   factor_cnt = 0;
    size_t   total = 0;      /* total number of prime factors (with multiplicity) */

    uint64_t tmp = n;

    /* Handle the factor 2 separately to keep the loop simple */
    if ((tmp & 1ULL) == 0) {
        size_t e = 0;
        while ((tmp & 1ULL) == 0) { tmp >>= 1; ++e; }
        prime_list[factor_cnt] = 2;
        exp_list[factor_cnt++] = e;
        total += e;
    }

    /* Ensure we have enough primes precomputed up to sqrt(tmp) */
    uint64_t max_check = 0;
    if (tmp > 1) {
        /* Find an upper bound for sqrt(tmp) without floating point */
        uint64_t low = 1, high = tmp;
        while (low <= high) {
            uint64_t mid = (low + high) >> 1;
            if (mid == 0) { high = mid - 1; continue; }
            if (mid > tmp / mid) { high = mid - 1; }
            else { max_check = mid; low = mid + 1; }
        }
        ensure_primes_up_to(max_check);
    }

    /* Trial division using the prime list */
    for (size_t i = 0; i < primes_len && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (p == 2) continue;          /* already processed */
        if (tmp % p == 0) {
            size_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            prime_list[factor_cnt] = p;
            exp_list[factor_cnt++] = e;
            total += e;
        }
    }

    /* If something remains > 1, it is a prime factor larger than sqrt(original) */
    if (tmp > 1) {
        prime_list[factor_cnt] = tmp;
        exp_list[factor_cnt++] = 1;
        ++total;
    }

    /* Condition (1): length must be even */
    if (total & 1U) return false;

    size_t half = total >> 1;         /* total/2  */
    size_t count_so_far = 0;
    uint64_t mid1 = 0, mid2 = 0;

    /* Find the two middle factors (0‑based indices half-1 and half). */
    for (size_t i = 0; i < factor_cnt; ++i) {
        count_so_far += exp_list[i];
        if (mid1 == 0 && count_so_far > half - 1) mid1 = prime_list[i];
        if (mid2 == 0 && count_so_far > half) { mid2 = prime_list[i]; break; }
    }

    return mid1 != mid2;
}

/* ------------------------------------------------------------------ */
/* Public API: compute the n‑th term of A362617.                       */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0; safety return */

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        ++last_tested;             /* next integer to examine */

        if (good_factorization(last_tested))
            seq_push((seq_t)last_tested);
    }

    return seq_cache[n - 1];
}
