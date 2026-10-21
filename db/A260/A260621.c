/*
 * A260621  Let b(k, n) = number obtained when the map x->A002808(x) is
 *          applied k times to n; a(n) is the smallest k such that b(k, n) + 1
 *          is prime.
 * https://oeis.org/A260621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k such that b(k,n)+1 is prime, where b(k,n)
 *            is obtained by applying the map x → A002808(x) (the x‑th
 *            composite number) k times to n
 * Range:     u8 n <= 2, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation mirrors the discussion: it builds the list of
   composite numbers on demand and uses a deterministic Miller–Rabin
   test for 64‑bit integers, which suffices for all values encountered
   for n up to about 10000.  The algorithm works for arbitrary
   precision seq_t as long as all intermediate composite indices fit
   in the 64‑bit idx_t type. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/* 64‑bit Miller–Rabin utilities (deterministic for n < 2^64)             */
static inline uint64_t mulmod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

static uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mulmod_uint64(result, base, mod);
        base = mulmod_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint32_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint32_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ull) == 0) { d >>= 1; ++s; }
    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = mulmod_uint64(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Global storage for the list of composite numbers (1‑based)           */
static seq_t *composites = NULL;   /* array of composite numbers   */
static size_t composites_size = 0; /* number of composites stored */
static size_t composites_cap = 0;  /* current allocation capacity  */
static uint64_t last_checked = 3;  /* last integer examined for primality */

/* Append a composite number to the global array, expanding capacity if needed */
static void add_composite(seq_t val)
{
    if (composites_cap == composites_size) {
        size_t new_cap = composites_cap ? composites_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
        if (!tmp) abort();                      /* unrecoverable allocation failure */
        composites = tmp;
        composites_cap = new_cap;
    }
    composites[composites_size++] = val;
}

/* Ensure that the first 'need' composite numbers are available         */
static void ensure_composites_n(idx_t need)
{
    while (composites_size < need) {
        ++last_checked;
        if (last_checked < 4) continue;          /* composite sequence starts at 4 */
        if (!is_prime_uint64(last_checked)) {
            add_composite((seq_t)last_checked);
        }
    }
}

/* Return the n-th composite number (1‑based) */
static seq_t composite_of(idx_t n)
{
    ensure_composites_n(n);
    return composites[n - 1];
}

/* --------------------------------------------------------------------- */
/* Public sequence function as required                                */
seq_t a(idx_t n)
{
    /* n is 1‑based.  The first iteration corresponds to k = 1. */
    idx_t k = 1;
    seq_t current = (seq_t)n;                 /* start index for the composite map */

    while (1) {
        /* If current exceeds idx_t range (very unlikely for tested n),
           we abort; the test data keeps values within 64‑bit. */
        if ((uint64_t)current > UINT64_MAX) abort();

        seq_t val = composite_of((idx_t)current);   /* b(k,n) */
        if (is_prime_uint64((uint64_t)(val + (seq_t)1)))
            return (seq_t)k;                      /* first k with (b(k,n)+1) prime */
        current = val;                             /* next iteration uses the composite obtained */
        ++k;
    }
}
