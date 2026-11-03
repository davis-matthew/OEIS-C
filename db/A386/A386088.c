/*
 * A386088  Primes having only {1, 2, 3, 7} as digits.
 * https://oeis.org/A386088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 225, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9739 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes whose decimal digits belong to {1,2,3,7} */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
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

/* Allowed decimal digits in ascending order. */
static const uint8_t allowed_digits[4] = {1, 2, 3, 7};

/* Cache of primes already found. */
static seq_t *prime_cache = NULL;   /* array of seq_t */
static size_t cache_cap   = 0;       /* allocated capacity */
static size_t cache_len   = 0;       /* number of primes stored */

/* Current index representation of the candidate number.  index 0 = '1', 1 = '2', … */
static uint8_t *idx_arr      = NULL; /* array of indices into allowed_digits */
static size_t   idx_len      = 0;     /* number of digits in the current candidate */
static size_t   idx_cap      = 0;     /* allocated capacity for idx_arr */

/* Initialise static structures on first use. */
static void init_state(void)
{
    if (prime_cache == NULL) {
        /* initial space for 64 primes */
        ensure_capacity(64, &cache_cap, OEISC_VPP(&prime_cache), sizeof(seq_t));
    }
    if (idx_arr == NULL) {
        /* initial space for 8 digits */
        ensure_capacity(8, &idx_cap, OEISC_VPP(&idx_arr), sizeof(uint8_t));
        idx_len = 1;
        idx_arr[0] = 0; /* first digit '1' */
    }
}

/* Increment the digit index array, expanding the length when needed. */
static void increment_indices(void)
{
    int64_t pos = (int64_t)idx_len - 1; /* index of the least‑significant digit */

    while (pos >= 0) {
        if (idx_arr[pos] == 3) {  /* digit '7' → carry */
            idx_arr[pos] = 0;
            pos--;
        } else {
            idx_arr[pos]++;       /* simple increment */
            return;
        }
    }

    /* Completely carried over: create a new most‑significant digit (always '1'). */
    idx_len++;
    if (idx_len > idx_cap) {
        ensure_capacity(idx_len, &idx_cap, OEISC_VPP(&idx_arr), sizeof(uint8_t));
    }
    for (size_t i = 0; i < idx_len; ++i)
        idx_arr[i] = 0;
}

/* Build the integer value represented by the current digit indices. */
static seq_t current_value(void)
{
    seq_t val = 0;
    for (size_t i = 0; i < idx_len; ++i) {
        val = val * 10 + (seq_t)allowed_digits[idx_arr[i]];
    }
    return val;
}

/* Ensure that at least `target` primes have been cached. */
static void ensure_primes_upto(idx_t target)
{
    init_state();
    while (cache_len < target) {
        seq_t val = current_value();

        if (is_prime_big(val)) {
            if (cache_len >= cache_cap) {
                ensure_capacity(cache_len + 64, &cache_cap,
                                OEISC_VPP(&prime_cache), sizeof(seq_t));
            }
            prime_cache[cache_len++] = val;
        }
        increment_indices();
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0, but safe fallback */
    ensure_primes_upto(n);
    return prime_cache[n - 1];
}
