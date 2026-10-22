/*
 * A331682  One and all numbers whose prime indices are pairwise coprime and
 *          already belong to the sequence, where a singleton is always
 *          considered to be coprime.
 * https://oeis.org/A331682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A331682: numbers whose prime
   indices are pairwise coprime and already belong to the sequence. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Utilities for prime generation and gcd.  The implementation
   below uses its own simple integer gcd; no external utility
   is required. */
static inline int int_gcd(int a, int b)
{
    while (b) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a < 0 ? -a : a;
}

/* Prime sieve data */
static uint32_t *primes_arr = NULL;
static size_t primes_cap = 0;
static size_t primes_count = 0;

/* Sequence data */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static idx_t next_candidate = 2;

/* Current index of a prime, or -1 if not present in the sieve. */
static int prime_index(uint32_t p)
{
    size_t lo = 0, hi = primes_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint32_t val = primes_arr[mid];
        if (val < p) {
            lo = mid + 1;
        } else if (val > p) {
            hi = mid;
        } else {
            return (int)(mid + 1); /* 1‑based index */
        }
    }
    return -1;
}

/* Ensure prime sieve is filled up to a static limit. */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;

    /* Sieve limit: sufficient for n up to about 10 000. */
    const idx_t LIMIT = 2000000;
    unsigned char *sieve = (unsigned char*)malloc((size_t)LIMIT + 1);
    if (!sieve) return;            /* allocation failure: silent abort */

    memset(sieve, 1, (size_t)LIMIT + 1);
    sieve[0] = sieve[1] = 0;
    for (idx_t i = 2; (i * i) <= LIMIT; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 0;
        }
    }

    primes_cap = 200000;
    primes_arr = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    primes_count = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (sieve[i]) {
            if (primes_count >= primes_cap) {
                primes_cap <<= 1;
                primes_arr = (uint32_t*)realloc(primes_arr, primes_cap * sizeof(uint32_t));
            }
            primes_arr[primes_count++] = (uint32_t)i;
        }
    }
    free(sieve);
    done = true;
}

/* Ensure the sequence array can hold 'needed' elements. */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t newcap = seq_cap ? seq_cap << 1 : 16;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_arr, newcap * sizeof(seq_t));
    if (!tmp) return; /* keep old array on failure */
    seq_arr = tmp;
    seq_cap = newcap;
}

/* Check whether a given index value is already present in the sequence. */
static bool present_in_seq(int idx)
{
    /* The built‑in sequence elements are never larger than the indices
       we test here for indices up to the sieve limit, so a simple
       linear scan is adequate. */
    seq_t target = (seq_t)idx;
    for (size_t i = 0; i < seq_len; ++i) {
        if (seq_arr[i] == target) return true;
    }
    return false;
}

/* Determine whether a candidate integer satisfies the definition. */
static bool candidate_ok(uint64_t n)
{
    if (n == 1) return true;

    uint64_t temp = n;
    int used_idx[64];
    size_t used_cnt = 0;

    for (size_t i = 0; i < primes_count &&
         (uint64_t)primes_arr[i] * (uint64_t)primes_arr[i] <= temp; ++i) {
        uint32_t p = primes_arr[i];
        if (temp % p != 0) continue;

        /* Count multiplicity */
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        int idx = prime_index(p);
        if (idx <= 0) return false;          /* prime not in sieve */

        if (idx > 1 && exp > 1) return false; /* repeated index > 1 */

        if (!present_in_seq(idx)) return false; /* required index not in seq */

        if (idx > 1) {
            for (size_t j = 0; j < used_cnt; ++j) {
                if (int_gcd(idx, used_idx[j]) > 1) return false;
            }
            used_idx[used_cnt++] = idx;
        }
    }

    if (temp > 1) {                          /* remaining large prime factor */
        uint32_t p = (uint32_t)temp;
        int idx = prime_index(p);
        if (idx <= 0) return false;
        if (!present_in_seq(idx)) return false;
        if (idx > 1) {
            for (size_t j = 0; j < used_cnt; ++j) {
                if (int_gcd(idx, used_idx[j]) > 1) return false;
            }
        }
    }
    return true;
}

/* Public function required for OEIS evaluation. */
seq_t a(idx_t n)
{
    if (seq_len == 0) {
        init_primes();
        ensure_seq_cap(1);
        seq_arr[0] = (seq_t)1;
        seq_len = 1;
        next_candidate = 2;
    }

    while (seq_len < (size_t)n) {
        uint64_t cand = (uint64_t)next_candidate++;
        if (candidate_ok(cand)) {
            ensure_seq_cap(seq_len + 1);
            seq_arr[seq_len++] = (seq_t)cand;
        }
    }
    return seq_arr[n - 1];
}
