/*
 * A296520  Multidigit primes in which the largest digit appears only once.
 * https://oeis.org/A296520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   digit_frequencies, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multidigit primes with the largest digit occurring exactly once.
   a(n) is the n‑th prime >= 10 whose largest decimal digit appears
   only once.  The sequence starts with 13, 17, 19, 23, 29 ...   */

/* The implementation maintains a cache of the primes already found
   and extends it lazily until at least n terms are present.  The
   detection of a qualifying prime uses `is_prime_big` and the
   frequency table supplied by `digit_frequencies`.  The algorithm
   has linear time in the index n and is valid for any `seq_t`
   implementation provided by the build. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_frequencies.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

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

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

static seq_t *cache = NULL;            /* cached qualifying primes   */
static size_t cache_size = 0;          /* number of terms stored    */
static size_t cache_cap  = 0;          /* allocated capacity        */
static seq_t next_candidate = 10;      /* next candidate to test    */

/* Ensure that the cache holds at least n terms (n >= 1). */
static void ensure_cache_size(idx_t n)
{
    while (cache_size < (size_t)n) {
        /* Search for the next suitable prime. */
        while (true) {
            if (next_candidate >= (seq_t)10 && is_prime_big(next_candidate)) {
                unsigned int freq[10] = {0};
                digit_frequencies(next_candidate, freq);

                /* Identify the largest digit present. */
                int max_digit = -1;
                for (int d = 9; d >= 0; --d) {
                    if (freq[d] > 0) {
                        max_digit = d;
                        break;
                    }
                }

                /* Olargest digit must appear exactly once. */
                if (max_digit >= 0 && freq[max_digit] == 1)
                    break;          /* found a qualifying prime */
            }
            ++next_candidate;           /* try the next integer   */
        }

        /* Store the qualifying prime in the cache. */
        ensure_capacity(cache_size + 1, &cache_cap,
                        OEISC_VPP(&cache), sizeof(seq_t));
        cache[cache_size++] = next_candidate;

        /* Continue searching from the following integer. */
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for indices < 1 */

    ensure_cache_size(n);
    return cache[n - 1];
}
