/*
 * A244466  Nonprimes n such that mu(phi(n)) = 1.
 * https://oeis.org/A244466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 680, u32 n <= 11320, u64 n <= 11320,
 *            u128 n <= 11320, big any n
 * Validated: standard, pass, 70 terms through n = 9282 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244466:
   a(n) = the nth non‑prime integer m such that μ(φ(m)) = 1.
   The implementation scans increasing integers, tests non‑primality,
   computes φ(m) by factorisation, then computes μ(φ(m)) from a second
   factorisation of φ(m).  Results are cached in a static array so that
   repeated calls are fast. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
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

/* Helper to compute φ(x) for an unsigned integer x <= 2^64. */
static seq_t phi_uint64(idx_t x)
{
    seq_t phi = (seq_t)x;
    idx_t t = x;
    idx_t d = 2;
    while (d <= t / d) {
        if (t % d == 0) {
            phi -= phi / (seq_t)d;
            while (t % d == 0) t /= d;
        }
        d += (d == 2 ? 1 : 2);
    }
    if (t > 1) {
        phi -= phi / (seq_t)t;
    }
    return phi;
}

/* Helper to compute μ(y) for an unsigned integer y <= 2^64. */
static int mobius_uint64(idx_t y)
{
    int primes = 0;
    bool square_free = true;
    idx_t t = y;
    idx_t d = 2;
    while (d <= t / d && square_free) {
        if (t % d == 0) {
            primes++;
            int exp = 0;
            while (t % d == 0) {
                t /= d;
                exp++;
                if (exp > 1) {
                    square_free = false;
                    break;
                }
            }
        }
        d += (d == 2 ? 1 : 2);
    }
    if (square_free && t > 1) {
        primes++;          /* last prime factor */
    }
    if (!square_free) return 0;
    return (primes & 1) ? -1 : 1;
}

seq_t a(idx_t n)
{
    static seq_t *found = NULL;
    static size_t found_cap = 0;
    static size_t found_len = 0;
    static idx_t next = 1;            /* next integer to examine */

    while (found_len < n) {
        idx_t cand = next++;
        /* 1 is treated as composite; skip primes */
        if (!(cand == 1 || !is_prime(cand))) continue;

        /* compute φ(cand) */
        seq_t phi = phi_uint64(cand);

        /* compute μ(φ) */
        if (mobius_uint64((idx_t)phi) == 1) {
            /* store term */
            ensure_capacity(found_len + 1, &found_cap,
                            OEISC_VPP(&found), sizeof(seq_t));
            found[found_len++] = (seq_t)cand;
        }
    }

    return found[n - 1];
}
