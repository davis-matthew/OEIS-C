/*
 * A322121  Composite numbers m such that b^(m-1) == 1 (mod (b^2-1)*m) has a
 *          solution b.
 * https://oeis.org/A322121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   is_prime, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A322121: Composite odd m not divisible by 3 that
   admit a base b > 1 with b^(m-1) ≡ 1 (mod (b^2−1)*m). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

static seq_t *cache = NULL;          /* cache of computed terms */
static size_t cache_cap = 0;          /* allocated capacity of cache */
static idx_t  cache_len = 0;          /* number of terms stored */
static idx_t  next_candidate = 5;     /* next odd integer to test */

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    /* Ensure sufficient space in the cache. */
    if (n > cache_len)
        ensure_cache(n, &cache, &cache_cap);

    /* Generate terms until the cache has at least n elements. */
    while (cache_len < n) {
        /* Skip non‑odd, multiples of 3, or primes. */
        if ((next_candidate & 1ULL) == 0) {   /* even */
            next_candidate += 1;
            continue;
        }
        if ((next_candidate % 3ULL) == 0) {   /* divisible by 3 */
            next_candidate += 2;
            continue;
        }
        if (is_prime(next_candidate)) {       /* prime */
            next_candidate += 2;
            continue;
        }

        idx_t m = next_candidate;
        bool ok = false;

        /* Search for a base b that satisfies the congruence. */
        for (idx_t b = 2; b < m; ++b) {
            /* Compute modulus M = (b^2 – 1) * m using 128‑bit arithmetic. */
            unsigned __int128 prod = (unsigned __int128)b * b;
            prod -= 1;
            prod *= m;

            /* If M exceeds the range of seq_t, skip this b. */
            if (prod > (unsigned __int128)~(seq_t)0)
                continue;

            seq_t mod = (seq_t)prod;
            if (powmod((seq_t)b, m - 1, mod) == 1) {
                ok = true;
                break;
            }
        }

        if (ok)
            cache[cache_len++] = (seq_t)m;

        next_candidate += 2;       /* advance to the next odd integer */
    }

    return cache[n - 1];
}
