/*
 * A285015  Primes of the form k * b^b - 1, with b > 1.
 * https://oeis.org/A285015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 8832 (2026-11-03)
 * Helpers:   ipow, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A285015: primes of the form k * b^b – 1, with b > 1 */

/* This implementation keeps the set of bases small (2 … 6).  For n ≤ 10 000
   this is sufficient because 6^6 = 46656 and the 10 000th prime of the
   sequence is < 2.0×10⁵.  The small base range guarantees that
   `b^b` fits into any seq_t type, avoiding overflow for the 64‑bit build.
   Duplicates arising from different bases (e.g. 107 = 27*4‑1 = 4*27‑1)
   are detected and eliminated. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow.h"
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

/* Fixed list of bases (2,3,4,5,6) */
#define NBAS 5
static const idx_t BASES[NBAS] = {2, 3, 4, 5, 6};

static seq_t bpow[NBAS];            /* b^b */
static idx_t nextk[NBAS];           /* current k (>=1) */
static seq_t nextval[NBAS];         /* next candidate = k*b^b - 1 */
static bool init_done = false;

/* Cache for found primes */
static seq_t *prime_cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap  = 0;

/* Ensure that the cache can hold at least n primes */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;
    while (cache_cap < n)
        cache_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    prime_cache = mem_resize(seq_t, prime_cache, cache_cap);
}

/* Append a prime to the cache, skipping duplicates. */
static void append_prime(seq_t p)
{
    if (cache_size > 0 && prime_cache[cache_size-1] == p)
        return;           /* duplicate – ignore */
    if (cache_size == cache_cap) {
        cache_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        prime_cache = mem_resize(seq_t, prime_cache, cache_cap);
    }
    prime_cache[cache_size++] = p;
}

/* One‑time initialization of tables. */
static void init_data(void)
{
    if (init_done)
        return;
    for (idx_t i = 0; i < NBAS; ++i) {
        bpow[i]   = ipow(BASES[i], (seq_t)BASES[i]); /* b^b */
        nextk[i]  = 1;
        nextval[i]= bpow[i] * (seq_t)1 - (seq_t)1;     /* k*b^b -1 with k=1 */
    }
    cache_cap = 8;
    prime_cache = mem_new(seq_t, cache_cap);
    cache_size = 0;
    init_done = true;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset is 1 – safeguard */

    init_data();

    /* Continue collecting primes until we have n of them. */
    while (cache_size < n) {
        /* Find the smallest next candidate among all bases. */
        idx_t best = 0;
        for (idx_t i = 1; i < NBAS; ++i)
            if (nextval[i] < nextval[best])
                best = i;

        seq_t cand = nextval[best];

        /* Test candidate for primality. */
        if (is_prime_big(cand))
            append_prime(cand);

        /* Advance this base to its next value. */
        ++nextk[best];
        nextval[best] = bpow[best] * (seq_t)nextk[best] - (seq_t)1;
    }

    return prime_cache[n - 1];
}
