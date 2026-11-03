/*
 * A396555  Primes p such that EulerPhi(p-1) is not a power of 2.
 * https://oeis.org/A396555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 6507, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9229 (2026-11-03)
 * Helpers:   is_pow2, mem, next_prime, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation for A396555: primes p such that EulerPhi(p-1) is NOT a power of 2 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_pow2.h"
#include "../utils/next_prime.h"
#include "../utils/totient_seq.h"
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

seq_t a(idx_t n) {
    /* static cache of qualifying primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t cur_prime = 2;        /* last prime processed */

    /* Ensure we have at least n terms in cache (index n-1) */
    while (cache_len < n) {
        /* get next prime */
        cur_prime = next_prime(cur_prime + 1);

        /* compute Euler phi of p-1 */
        seq_t phi = totient_seq((seq_t)(cur_prime - 1));

        /* check condition */
        if (!is_pow2(phi)) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                /* double capacity or set to 1 if empty */
                size_t new_cap = cache_cap ? cache_cap * 2 : 1;
                ensure_capacity(new_cap, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)cur_prime;
        }
    }
    return cache[n - 1];
}
