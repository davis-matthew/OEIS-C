/*
 * A370495  Oblong numbers of the form (k-1)*k where k is the product of an
 *          even number of distinct primes.
 * https://oeis.org/A370495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k*(k-1) where k is the product of an even number of
 *            distinct primes. The sequence is indexed from 1 with a(1) = 0
 * Range:     u8 n <= 5, u16 n <= 78, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9986 (2026-11-03)
 * Helpers:   mem, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/smallest_prime_factor.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Static cache to allow arbitrary call order. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static uint64_t next_k   = 2;   /* next k to test (k > 1)   */

    /* Initialise cache on first call. */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = mem_new(seq_t, 1);
        if (cache == NULL)
            return (seq_t)0;            /* failure: return 0 */
        cache[0] = (seq_t)0;           /* a(1) = 0 */
        cache_len = 1;
    }

    /* Generate terms until we have at least n terms. */
    while (cache_len < (size_t)n) {
        uint64_t x = next_k;
        int      omega = 0;
        bool     squarefree = true;

        /* Factorise k using the smallest‑prime‑factor helper. */
        while (x > 1 && squarefree) {
            uint64_t p = smallest_prime_factor(x);
            int      exp = 0;
            while (x % p == 0) {
                x /= p;
                exp++;
            }
            if (exp > 1) {                 /* not square‑free */
                squarefree = false;
                break;
            }
            omega++;                       /* count distinct prime */
        }

        if (squarefree && (omega & 1) == 0) {
            /* k satisfies the condition – add to cache. */
            seq_t val = (seq_t)next_k * ((seq_t)next_k - 1);
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = val;
        }

        next_k++;   /* advance to the next candidate k */
    }

    return cache[(size_t)n - 1];
}
