/*
 * A274504  Primes with Kronecker symbol (p|14) != 1.
 * https://oeis.org/A274504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 956 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p with Kronecker symbol (p|14) != 1 */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Helper: sign of (p|2) given by (−1)^{(p^2−1)/8} */
static inline int sig2(idx_t p)
{
    uint8_t r = p % 8;
    return (r == 1 || r == 7) ? 1 : -1;
}

/* Helper: sign of (p|7); 1 if p mod 7 is a quadratic residue, else -1. */
static inline int sig7(idx_t p)
{
    uint8_t r = p % 7;
    return (r == 1 || r == 2 || r == 4) ? 1 : -1;
}

/* Return true if p should be in the sequence. */
static inline bool keep_prime(idx_t p)
{
    if (p == 2 || p == 7)          /* (p|14)=0 for these primes */
        return true;
    int s2 = sig2(p);
    int s7 = sig7(p);
    return s2 * s7 != 1;           /* symbol ≠ 1  -> keep */
}

/* Cached list of terms */
static seq_t *cache    = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t last_checked = 1;   /* number just checked */

/* a(n) returns the n-th element (1‑indexed).  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    while (cache_len < n) {
        ++last_checked;               /* next integer to examine */
        if (!is_prime(last_checked))  /* skip composites */
            continue;

        if (keep_prime(last_checked)) {   /* add to cache */
            if (cache_len + 1 > cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
