/*
 * A280266  Primes such that the previous prime plus the next prime plus 1 is
 *          also prime.
 * https://oeis.org/A280266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 862 (2026-11-03)
 * Helpers:   is_prime, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A280266: primes p such that
   the previous prime + the next prime + 1 is also prime.
   The function builds primes on demand and caches the sequence
   values, allowing calls in any order. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/is_prime.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Helper to grow a dynamic array */
#ifdef __cplusplus
template <class T>
static void ensure_cap(size_t needed, size_t *cap, T **ptr, size_t elem_size) {
    if (needed > *cap) {
        size_t new_cap = *cap ? *cap * 2 : 8;
        while (new_cap < needed) new_cap *= 2;
        T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
        if (!tmp) abort();          /* allocation failed */
        *ptr  = tmp;
        *cap  = new_cap;
    }
}
#else
static void ensure_cap(size_t needed, size_t *cap, void **ptr, size_t elem_size) {
    if (needed > *cap) {
        size_t new_cap = *cap ? *cap * 2 : 8;
        while (new_cap < needed) new_cap *= 2;
        void *tmp = realloc(*ptr, new_cap * elem_size);
        if (!tmp) abort();          /* allocation failed */
        *ptr  = tmp;
        *cap  = new_cap;
    }
}
#endif

/* Storage for primes seen so far */
static uint64_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;

/* Storage for the sequence values */
static seq_t *seq = NULL;
static size_t seq_len = 0, seq_cap = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset is 1 – undefined for n=0 */

    while (seq_len < (size_t)n) {
        /* Generate the next prime */
        uint64_t next;
        if (primes_len == 0)
            next = next_prime(2);    /* first prime is 2 */
        else
            next = next_prime(primes[primes_len - 1] + 1);

        ensure_cap(primes_len + 1, &primes_cap, OEISC_VPP(&primes),
                   sizeof(uint64_t));
        primes[primes_len++] = next;

        /* When we have at least three primes, check the middle one */
        if (primes_len >= 3) {
            uint64_t prev  = primes[primes_len - 3];
            uint64_t curr  = primes[primes_len - 2];
            uint64_t nextp = primes[primes_len - 1];

            idx_t sum = (idx_t)prev + (idx_t)nextp + (idx_t)1;
            if (is_prime(sum)) {
                ensure_cap(seq_len + 1, &seq_cap, OEISC_VPP(&seq),
                           sizeof(seq_t));
                seq[seq_len++] = curr;
            }
        }
    }

    return seq[n - 1];
}
