/*
 * A351170  Consider the primes of the form p(m)=m^2+1 such that p(m+2) is
 *          also prime for some m. The sequence lists the sums p(m) + p(m+2).
 * https://oeis.org/A351170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*(m_n+1)^2+4 where m_n is the n‑th integer m such that
 *               m^2+1 and (m+2)^2+1 are prime.
 * Computation: iterate m, test primes with isPrimeGeneric, memoise results.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
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

seq_t a(idx_t n)
{
    static seq_t *vals   = NULL;
    static size_t  vals_len = 0;
    static size_t  vals_cap = 0;
    static uint64_t next_m  = 1;     /* next m to test */

    if (n == 0) return 0;           /* offset starts at 1 */

    size_t target = (size_t)n;
    if (vals_len < target) {
        if (vals_cap < target) {
            size_t new_cap = vals_cap ? vals_cap * 2 : 1;
            if (new_cap < target) new_cap = target;
            ensure_capacity(new_cap, &vals_cap, OEISC_VPP(&vals), sizeof(seq_t));
        }
        while (vals_len < target) {
            seq_t m  = (seq_t)next_m;
            seq_t p1 = m * m + 1;
            seq_t p2 = (m + 2) * (m + 2) + 1;

            if (isPrimeGeneric(p1) && isPrimeGeneric(p2)) {
                seq_t mm1 = m + 1;
                seq_t val = 2 * (mm1 * mm1) + 4;
                vals[vals_len++] = val;
            }
            ++next_m;
        }
    }
    return vals[n - 1];
}
