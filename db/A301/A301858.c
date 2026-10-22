/*
 * A301858  Positive integers which can be written as the sum of two squares
 *          but cannot be written as x^2 + y^2 + 2*z^2 with x and y integers
 *          and z a nonzero integer.
 * https://oeis.org/A301858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n-th positive integer that can be expressed
 *              as a sum of two squares but cannot be expressed as
 *              x^2 + y^2 + 2*z^2 with z ≠ 0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --------- Integer square‑root (floor) for 64‑bit unsigned ---------- */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t low = 0, high = (idx_t)1 << 32;   /* 2^32 > sqrt(2^64) */
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if (mid <= n / mid) low = mid;
        else high = mid;
    }
    return low;
}

/* --------- Small prime sieve (dynamic, up to a given limit) --------- */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t    prime_cap   = 0;     /* capacity of prime_list   */
static size_t    prime_cnt   = 0;     /* number of primes stored  */
static idx_t     prime_limit = 0;     /* largest computed prime  */

static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* Allocate/combine to new size */
    if (!prime_list) {
        prime_cap = 128;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_cnt = 0;
    } else if (limit > prime_limit) {
        /* enlarge prime count array if needed */
        if (prime_cnt + 10 > prime_cap) {
            prime_cap = prime_cap * 2 + 10;
            prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        }
    }

    /* Simple Eratosthenes sieve up to 'limit' */
    size_t sieve_size = (size_t)limit + 1;
    bool *sieve = (bool *)calloc(sieve_size, sizeof(bool));
    if (!sieve) return;  /* out of memory: abort */

    for (idx_t i = 2; i <= limit; ++i) {
        if (!sieve[(size_t)i]) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = (uint64_t)i;

            if (i > limit / i) continue;
            for (idx_t j = i * i; j <= limit; j += i) sieve[(size_t)j] = true;
        }
    }
    free(sieve);
    prime_limit = limit;
}

/* --------- Check whether n is a sum of two squares -------------- */
static bool is_sum_two_squares(idx_t n)
{
    if (n == 0) return true;  /* 0^2 + 0^2 */

    idx_t temp = n;
    idx_t sqrt_temp = isqrt_u64(temp);
    ensure_primes_upto(sqrt_temp);

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= sqrt_temp; ++i) {
        idx_t p = prime_list[i];
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        if ((p & 3ULL) == 3ULL && (exp & 1)) return false;  /* odd exp of 3 mod 4 prime */
        sqrt_temp = isqrt_u64(temp);
    }
    if (temp > 1 && (temp & 3ULL) == 3ULL) return false;
    return true;
}

/* --------- Cache for the sequence ---------------------------------- */
static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;      /* number of terms stored */

static void ensure_cache_capacity(void)
{
    if (seq_cap == 0) {
        seq_cap = 8;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    } else if (seq_len >= seq_cap) {
        seq_cap *= 2;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
}

/* --------- Main sequence function ---------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0 */

    static idx_t   last_checked = 0;
    static bool    finished = false;

    /* Build terms until we reach n or we decide no more terms exist. */
    while (seq_len < n && !finished) {
        ++last_checked;  /* candidate number */

        /* Only numbers that are sums of two squares can be in the set. */
        if (!is_sum_two_squares(last_checked)) continue;

        /* Check whether it can be expressed as x^2 + y^2 + 2*z^2 with z≠0. */
        bool bad = false;
        idx_t zmax = isqrt_u64(last_checked / 2);
        for (idx_t z = 1; z <= zmax; ++z) {
            idx_t t = last_checked - 2 * z * z;
            if (t == 0) { bad = true; break; }         /* (0,0,z) */
            if (is_sum_two_squares(t)) { bad = true; break; }
        }

        if (!bad) {
            ensure_cache_capacity();
            seq_cache[seq_len++] = (seq_t)last_checked;
        }

        /* After scanning up to 1 000 000 with no new terms, assume the sequence is finite. */
        if (last_checked > 1000000ULL && seq_len < 4) {
            finished = true;
        }
    }

    if (seq_len < n) return (seq_t)0;  /* no such nth term (sequence presumed finite) */
    return seq_cache[n - 1];
}
