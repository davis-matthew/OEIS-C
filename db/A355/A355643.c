/*
 * A355643  Numbers k having a divisor d such that d+k/d is prime.
 * https://oeis.org/A355643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 2971, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A355643: numbers k having a divisor d such that d + k/d is prime,
   in ascending order.  The function returns the n‑th term (1‑based). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/floor_sqrt.h"

/* cache stores the sequence terms computed so far */
static seq_t *cache = NULL;          /* array of terms           */
static size_t cache_size = 0;        /* number of terms stored   */
static size_t cache_cap  = 0;        /* current capacity         */
/* next integer to test, starting from 1 */
static idx_t next_candidate = 1;

/* Append a new term to the cache, reallocating if necessary.
   The cast to (seq_t *) is required for C++ where seq_t may be a class. */
static inline void add_term(seq_t value)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = value;
}

/* Generate terms until at least n terms are stored in the cache. */
static void generate_up_to(idx_t n)
{
    while (cache_size < n) {
        seq_t k = (seq_t)next_candidate;
        bool found = false;
        idx_t sqrt_k = (idx_t)floor_sqrt(k);
        for (idx_t d = 1; d <= sqrt_k; ++d) {
            if ((k % (seq_t)d) == 0) {
                seq_t e = k / (seq_t)d;
                seq_t sum = (seq_t)d + e;
                if (isPrimeGeneric(sum)) {
                    add_term(k);
                    found = true;
                    break;
                }
            }
        }
        ++next_candidate;
        (void)found; /* value used only for side‑effects */
    }
}

/* Return the n‑th term (1‑based index) of A355643.  The function works for
   any order of calls; it memoises results in a static cache. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, placeholder */
    generate_up_to(n);
    return cache[n - 1];
}
