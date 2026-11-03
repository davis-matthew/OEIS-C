/*
 * A386168  Primes having only {2, 7, 8, 9} as digits.
 * https://oeis.org/A386168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 79, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 6721 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the nth prime whose decimal digits are only 2, 7, 8 or 9. */
/*  The implementation generates the admissible numbers in increasing
 *  order and keeps a cache of the primes found.  When a(n) is called
 *  the cache is extended until it contains at least n terms.       */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

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

/* digits allowed in a term */
static const unsigned char allowed[4] = {2, 7, 8, 9};

/* state of the number generator – works like a counter in base 4 */
static size_t gen_idx[20] = {0};   /* indices into allowed digit array   */
static size_t gen_len = 1;         /* current number of digits           */

/* return the next admissible integer (with digits from allowed[])
 * and increment the internal counter. */
static seq_t next_candidate(void)
{
    seq_t val = 0;
    for (size_t i = 0; i < gen_len; ++i)
        val = val * 10 + allowed[gen_idx[i]];

    /* increment counter in base‑4 */
    size_t pos = gen_len - 1;
    while (1) {
        ++gen_idx[pos];
        if (gen_idx[pos] < 4)
            break;
        gen_idx[pos] = 0;
        if (pos == 0) {                /* overflow -> increase length   */
            ++gen_len;
            if (gen_len > 20)          /* safety check – should not happen */
                break;
            for (size_t k = 0; k < gen_len; ++k)
                gen_idx[k] = 0;
            break;
        }
        --pos;
    }
    return val;
}

/* sequence cache */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_cnt = 0;
static bool cache_init = false;

/* main sequence routine */
seq_t a(idx_t n)
{
    if (!cache_init) {
        ensure_capacity(8, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache_init = true;
    }

    /* grow cache if needed */
    while (cache_cnt < (size_t)n) {
        seq_t cand = next_candidate();
        if (is_prime_big(cand)) {
            if (cache_cnt >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 8;
                ensure_capacity(new_cap, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
            }
            cache[cache_cnt++] = cand;
        }
    }
    return cache[n - 1];
}
