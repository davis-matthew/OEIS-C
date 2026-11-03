/*
 * A348855  a(1) = 1. If a(n) is prime, a(n+1) = 2*a(n) + 1. If a(n) is not
 *          prime, a(n+1) = least prime not already in the sequence.
 * https://oeis.org/A348855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1662, u32 n <= 10751, u64 n <= 10751,
 *            u128 n <= 10751, big any n
 * Validated: standard, pass, 76 terms through n = 8973 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A348855.
   a(1) = 1.
   For n ≥ 2, let a(n-1) = x.
   If x is prime then a(n) = 2·x + 1.
   Otherwise a(n) = smallest prime not yet appearing in the sequence. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static int compare_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* --- static data --- */
static seq_t *seq_cache = NULL;           /* 1‑based cache, seq_cache[0] unused */
static idx_t seq_last = 0;                /* highest index computed */
static size_t seq_capacity = 0;           /* allocated number of slots */

static seq_t *seen_primes = NULL;         /* primes that have already been emitted */
static size_t seen_count = 0, seen_cap = 0;
static seq_t next_prime_candidate = (seq_t)2;   /* next integer to test as a prime */

/* Ensure seq_cache has space for at least 'need' indices (including 0). */
static void ensure_cache_capacity(idx_t need)
{
    if ((size_t)need + 1 <= seq_capacity) return;
    grow_seq(&seq_cache, &seq_capacity, (size_t)need + 1);
}

/* Ensure seen_primes has room for at least 'need' elements. */
static void ensure_seen_capacity(size_t need)
{
    if (need <= seen_cap) return;
    grow_seq(&seen_primes, &seen_cap, need);
}

/* Return true if 'p' is already stored in seen_primes. */
static bool prime_already_seen(const seq_t *p)
{
    for (size_t i = 0; i < seen_count; ++i)
        if (compare_seq_t(&seen_primes[i], p) == 0)
            return true;
    return false;
}

/* Record that prime 'p' has been seen. */
static void mark_prime_seen(const seq_t *p)
{
    ensure_seen_capacity(seen_count + 1);
    seen_primes[seen_count++] = *p;
}

/* Find the smallest prime that has not yet appeared. */
static seq_t next_unused_prime(void)
{
    seq_t candidate = next_prime_candidate;
    for (;;) {
        if (!is_prime_big(candidate)) {
            candidate = candidate + (seq_t)1;
            continue;
        }
        if (!prime_already_seen(&candidate)) {
            /* remember this prime for future calls */
            mark_prime_seen(&candidate);
            next_prime_candidate = candidate + (seq_t)1;
            return candidate;
        }
        candidate = candidate + (seq_t)1;
    }
}

/* Build the sequence up to index 'n' (inclusive). */
static void build_to(idx_t n)
{
    if (n <= seq_last) return;
    for (idx_t i = seq_last + 1; i <= n; ++i) {
        seq_t prev = seq_cache[i - 1];
        seq_t nxt;
        if (is_prime_big(prev))
            nxt = prev * (seq_t)2 + (seq_t)1;
        else
            nxt = next_unused_prime();

        ensure_cache_capacity(i);
        seq_cache[i] = nxt;

        if (is_prime_big(nxt))
            mark_prime_seen(&nxt);

        ++seq_last;
    }
}

/* Public entry point requested by the build system. */
seq_t a(idx_t n)
{
    if (seq_cache == NULL) {                /* first call: initialise */
        ensure_cache_capacity(1);
        seq_cache[1] = (seq_t)1;            /* a(1) = 1, not prime */
        seq_last = 1;
    }
    build_to(n);
    return seq_cache[n];
}
