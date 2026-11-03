/*
 * A322517  Primes of form 1357913579... .
 * https://oeis.org/A322517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th prime obtained from successive prefixes of the
 *            repeating decimal 1357913579
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 1,
 *            big any n
 * Validated: standard, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper: free an array allocated by alloc_seq. */
static void free_seq(seq_t *p)
{
    mem_free(p);
}

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

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size) /* elem_size: sizeof(T) */
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, new_cap);
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

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined offset, return 0 for safety */

    /* static state shared between calls */
    static seq_t *prime_cache = NULL;  /* array of found primes       */
    static idx_t  cache_size  = 0;     /* number of stored primes     */
    static size_t cache_cap   = 0;     /* allocated capacity of cache */
    static seq_t  current     = 0;     /* current prefix value        */
    static idx_t  len         = 0;     /* number of digits processed  */
    static idx_t  digit_pos   = 0;     /* position within the pattern */
    const int pattern[5] = {1, 3, 5, 7, 9};

    /* initialise cache once */
    if (prime_cache == NULL) {
        cache_cap   = 16;
        prime_cache = alloc_seq(cache_cap);
    }

    /* generate primes until we have at least n of them */
    while (cache_size < n) {
        int digit = pattern[digit_pos % 5];
        digit_pos++;
        len++;
        current = current * 10 + (seq_t)digit;

        /* start looking for primes only after two digits */
        if (len >= 2 && is_prime_big(current)) {
            if ((size_t)cache_size >= cache_cap) {
                /* grow the cache to accommodate one more prime */
                ensure_capacity((size_t)(cache_size + 1), &cache_cap,
                               OEISC_VPP(&prime_cache), sizeof(seq_t));
            }
            prime_cache[cache_size++] = current;
        }
    }

    return prime_cache[n - 1];
}
