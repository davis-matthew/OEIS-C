/*
 * A378144  a(n) = P(n) * 2^floor(log_2(prime(n+1))) = A002110(n) *
 *          A000079(A098388(n+1)).
 * https://oeis.org/A378144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002110(n) * 2^floor(log_2(prime(n+1) - 1)) (Largest
 *            power of 2 times the primorial of n that is strictly less than
 *            the next primorial.)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 345 (2026-11-03)
 * Helpers:   ilog2, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/ilog2.h"
#include "../utils/pow2.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
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

/* Cache of primorials: primorials[i] = product of first i primes
   (primorials[0] = 1). */
static seq_t *primorials = NULL;
static size_t prim_cap = 0;      /* allocated capacity */
static size_t prim_len  = 0;      /* number of primorials computed */

/* Make sure primorials are computed up to index n (inclusive). */
static void
ensure_primorials_up_to(idx_t n)
{
    /* Initial allocation */
    if (prim_len == 0) {
        size_t need = (n + 1 > 1) ? n + 1 : 1;
        ensure_capacity(need, &prim_cap, OEISC_VPP(&primorials), sizeof(seq_t));
        primorials[0] = (seq_t)1;   /* 0 primes product is 1 */
        prim_len = 1;
    }

    /* Grow and compute new primorials if necessary */
    if (prim_len <= n) {
        size_t new_cap = prim_cap;
        while (new_cap <= n) new_cap <<= 1;
        ensure_capacity(new_cap, &prim_cap, OEISC_VPP(&primorials), sizeof(seq_t));

        /* Compute from the last known primorial up to n */
        for (idx_t i = prim_len; i <= n; ++i) {
            seq_t prime = (seq_t)get_prime(i);          /* i‑th prime */
            primorials[i] = seq_mul(primorials[i-1], prime);
        }
        prim_len = n + 1;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure primorial for n is ready */
    ensure_primorials_up_to(n);

    seq_t prim = primorials[n];
    uint32_t nextp   = get_prime(n + 1);              /* (n+1)-th prime */
    idx_t   e = ilog2((idx_t)(nextp - 1)); /* largest 2^e < nextp */
    seq_t pow = pow2(e);

    return seq_mul(prim, pow);
}
