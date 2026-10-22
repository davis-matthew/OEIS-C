/*
 * A387577  Numbers k such that it is not possible to choose a sequence of
 *          distinct integer partitions, one of each prime index of k (with
 *          multiplicity).
 * https://oeis.org/A387577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A387577: numbers k such that it is impossible to choose a sequence of distinct
   integer partitions – one of each prime index of k (with multiplicity).  */

/* The algorithm:
   • Pre‑compute all primes up to a fixed limit (here 100 000) and the one‑based
     index of each prime.
   • Compute the partition numbers p(i) for 0 ≤ i ≤ 200.  For an index i > 200
     the partition number is enormous, so no small exponent can exceed it;
     hence those indices cannot forbid a sequence of partitions.
   • For a given k, factorise it using the pre‑computed primes.
     For each prime factor p with exponent e and index i (prime_index[p]),
     check whether e > p(i).  If this happens for any factor, k belongs to
     A387577; otherwise it does not.
   • Sequence values are generated lazily and cached.  The function returns
     the n‑th term (a(1) is the first listed term). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- static data ---------- */
#define MAX_LIMIT 100000          /* enough for the expected range */
#define MAX_PART_INDEX 200       /* compute partitions only up to this index */

/* sieve data */
static uint32_t primes[MAX_LIMIT + 1];
static size_t prime_count = 0;
static uint32_t prime_index[MAX_LIMIT + 1] = {0};

/* partition numbers p(0..MAX_PART_INDEX) */
static uint64_t part[MAX_PART_INDEX + 1] = {0};

/* cache of sequence values      */
static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static uint64_t next_candidate = 1;

/* ---------- helpers ---------- */

/* initialise primes and partition numbers (thread‑safe for single thread) */
static void init(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* ------------ sieve of Eratosthenes ------------ */
    static unsigned char is_composite[MAX_LIMIT + 1] = {0};
    prime_count = 0;

    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_count] = i;
            prime_index[i] = (uint32_t)(prime_count + 1); /* 1‑based */
            ++prime_count;
            /* mark multiples */
            if ((uint64_t)i * i <= MAX_LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= MAX_LIMIT; j += i)
                    is_composite[(size_t)j] = 1;
            }
        }
    }

    /* ------------ partition numbers up to MAX_PART_INDEX ------------ */
    part[0] = 1;
    for (uint32_t n = 1; n <= MAX_PART_INDEX; ++n) {
        int64_t sum = 0;
        for (uint32_t k = 1; ; ++k) {
            uint32_t g1 = k * (3 * k - 1) / 2;
            if (g1 > n && k * (3 * k + 1) / 2 > n) break;
            int sign = (k & 1) ? 1 : -1;
            if (g1 <= n) sum += sign * (int64_t)part[n - g1];
            uint32_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= n) sum += sign * (int64_t)part[n - g2];
        }
        part[n] = (uint64_t)sum;
    }
}

/* test whether k is in A387577 */
static bool in_sequence(uint64_t k)
{
    if (k < 2) return false;
    uint64_t num = k;
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= num; ++i) {
        uint32_t p = primes[i];
        if (num % p != 0) continue;
        uint32_t exp = 0;
        while (num % p == 0) {
            num /= p;
            ++exp;
        }
        uint32_t idx = prime_index[p];
        if (idx <= MAX_PART_INDEX && (uint64_t)exp > part[idx])
            return true;
    }
    /* remaining factor (if any) is a prime > sqrt(original) */
    if (num > 1) {
        uint32_t idx = prime_index[(uint32_t)num];
        if (idx <= MAX_PART_INDEX && (uint64_t)1 > part[idx])
            return true; /* impossible, but kept for completeness */
    }
    return false;
}

/* grow the cache to at least n terms */
static void ensure_capacity(size_t n)
{
    while (seq_len < n) {
        ++next_candidate;
        if (in_sequence(next_candidate)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                seq_cache = tmp;
                seq_cap = new_cap;
            }
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
    }
}

/* ---------- required entry point ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined for safety */
    init();
    ensure_capacity((size_t)n);
    return seq_cache[(size_t)n - 1];
}
