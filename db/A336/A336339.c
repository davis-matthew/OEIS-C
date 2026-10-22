/*
 * A336339  Numbers composite(n) such that gcd(n,composite(n)) is even.
 * https://oeis.org/A336339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth composite number c such that gcd(n, c) is even,
 *            where c is the nth composite number in the natural order
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Helper: simple deterministic primality test for idx_t values */
static bool is_prime_idx(idx_t n) {
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (idx_t d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0ULL) return false;
    return true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* OEIS offsets start at 1 */
    if (n < 1) return (seq_t)0;

    /* State retained between calls */
    static seq_t *cache = NULL;   /* array of composite values */
    static size_t cache_cap = 0;  /* allocated capacity */
    static size_t cache_len = 0;  /* number of stored values */
    static idx_t nxt = 4;         /* next integer to test */
    static idx_t comp_ix = 0;     /* index in the list of composite numbers */

    /* Initialise cache lazily */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */
    }

    /* Generate enough terms to reach requested n */
    while (cache_len < n) {
        /* Skip primes */
        if (is_prime_idx(nxt)) {
            ++nxt;
            continue;
        }

        ++comp_ix;                /* nth composite number */

        /* Check gcd condition */
        seq_t g = gcd((seq_t)comp_ix, (seq_t)nxt);
        if ((g % 2) == 0) {
            /* Ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)nxt;
        }
        ++nxt;
    }
    return cache[n - 1];
}
