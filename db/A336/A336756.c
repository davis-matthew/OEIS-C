/*
 * A336756  Perimeters in increasing order of primitive integer-sided
 *          triangles whose sides a < b < c are in arithmetic progression.
 * https://oeis.org/A336756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_from_b
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A336756: perimeters p=3*b of primitive integer-sided
   triangles with sides in arithmetic progression (a<b<c, c-a=2*b-2*a).
   For each integer b≥3, the term p = 3*b repeats floor(phi(b)/2) times,
   where phi is Euler’s totient function.  Hence a(n) is the n‑th
   element of the multiset { 3*b | b≥3, repeated phi(b)/2 times }.
*/
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/phi_from_b.h"

/* Helper: generate primes up to a given limit (used for φ factorisation). */
static void build_primes_upto(idx_t limit, idx_t **primes_out, size_t *cnt_out)
{
    /* allocate provisional sieve */
    uint8_t *is_comp = (uint8_t *)malloc((size_t)(limit + 1));
    if (!is_comp) return;         /* out of memory – caller will fail */

    /* Sieve of Eratosthenes */
    for (size_t i = 0; i <= (size_t)limit; ++i) is_comp[i] = 0;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (!is_comp[(size_t)i])
            for (idx_t j = i * i; j <= limit; j += i) is_comp[(size_t)j] = 1;

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (!is_comp[(size_t)i]) ++cnt;

    /* Allocate and store primes */
    idx_t *pr = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!pr) { free(is_comp); return; }
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[(size_t)i]) pr[k++] = i;

    free(is_comp);
    *primes_out = pr;
    *cnt_out = cnt;
}

seq_t a(idx_t n)
{
    /* static cache of perimeters */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;

    /* state for generating perimeters */
    static idx_t cur_b = 3;
    static size_t rem   = 0;

    /* primes for φ computation (generated once) */
    static idx_t *prime_list = NULL;
    static size_t prime_cnt  = 0;
    if (!prime_list) build_primes_upto(1000000, &prime_list, &prime_cnt);

    if (n == 0) return 0;                /* undefined, but harmless */

    size_t target = (size_t)n;            /* up to 2^64, safe in size_t */

    while (size < target) {
        if (rem == 0) {
            seq_t phi = phi_from_b(cur_b, prime_list, prime_cnt);
            rem = (size_t)(phi / 2);    /* floor division */
            if (rem == 0) {             /* no perimeters for this b */
                ++cur_b;
                continue;
            }
        }
        /* ensure capacity */
        if (size >= cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* out of memory; abort gracefully */ return 0; }
            cache = tmp;
            cap = new_cap;
        }
        cache[size++] = 3 * (seq_t)cur_b;
        if (--rem == 0) ++cur_b;
    }

    return cache[target - 1];
}
