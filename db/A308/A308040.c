/*
 * A308040  Numbers k such that k - prevprime(k-1) is prime where prevprime(n)
 *          is the largest prime < n.
 * https://oeis.org/A308040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A(n) is the nth integer k ≥ 4 such that k - prevprime(k-1) is prime.  
   The routine generates terms on demand and caches them.  */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

/* Forward declarations for the optional allocation helper (used only
   if realloc casting is undesirable).  These are safe to omit if the
   implementation doesn't need them. */
#if 0
seq_t* seq_alloc_new_array(size_t n);
void seq_free_array(seq_t *p);
#endif

/* Static cache state.  */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 4;   /* next integer to test */

/* Ensure the cache can hold at least 'need' elements.  */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    /* For C++ the C-style cast converts void* to seq_t* safely.  */
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) {
        /* Allocation failed – aborting is safe for this context */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Returns the nth term of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    if (n == 0)
        abort();                 /* undefined for n < 1 */

    /* Generate terms until we have at least n terms. */
    while (cache_len < n) {
        idx_t prev = prev_prime(next_k - 1);   /* largest prime < k-1 */
        idx_t diff = next_k - prev;           /* k - prevprime(k-1) */
        if (is_prime(diff)) {                 /* diff itself is prime? */
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
