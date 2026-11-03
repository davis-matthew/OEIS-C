/*
 * A333634  Numbers with an even number of non-unitary prime divisors.
 * https://oeis.org/A333634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9959 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A333634: numbers with an even number of distinct primes occurring to power ≥2.
    The sequence is computed by testing each integer in succession and recording
    those for which the count of primes with exponent ≥2 is even (including 0). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

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

static seq_t *seq = NULL;      /* stored sequence values */
static size_t seq_len = 0;     /* current number of values stored */
static size_t seq_cap = 0;     /* current capacity of the array */
static uint64_t next_check = 1;/* next integer to test */

/*  Return true if x has an even number of distinct prime divisors with exponent ≥ 2. */
static inline bool has_property(uint64_t x)
{
    if (x <= 1) return true;        /* 1 has 0 such primes, which is even */

    uint64_t n = x;
    bool parity = false;            /* true -> odd count, false -> even count */

    /* handle factor 2 */
    if (n % 2 == 0) {
        int exp = 0;
        while (n % 2 == 0) {
            n /= 2;
            ++exp;
        }
        if (exp >= 2) parity = !parity;
    }

    /* trial division by odd numbers */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if (exp >= 2) parity = !parity;
        }
    }

    /* remaining n > 1 is prime with exponent 1 – it does not affect parity */
    return !parity;   /* true if count is even */
}

/* append val to the sequence array, resizing if necessary */
static inline void append_seq(uint64_t val)
{
    ensure_capacity(seq_len + 1, &seq_cap, OEISC_VPP(&seq), sizeof(seq_t));
    seq[seq_len++] = (seq_t)val;
}

/*  External linkage required by the build system */
seq_t a(idx_t n)
{
    /* Non‑positive indices are undefined; return 0 (not used). */
    if (n <= 0) return (seq_t)0;

    /* Generate sequence values until we have at least n elements. */
    while (seq_len < (size_t)n) {
        if (has_property(next_check))
            append_seq(next_check);
        ++next_check;
    }

    return seq[(size_t)n - 1];
}
