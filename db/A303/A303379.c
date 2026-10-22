/*
 * A303379  Primes p such that p == 1 (mod 6), p-2 is composite, and for all r
 *          in the range 5 <= r < p - 2, p-2 is not congruent to r (mod r*(r +
 *          1)/2).
 * https://oeis.org/A303379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 21, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A303379:
 * Find primes p ≡ 1 (mod 6) such that (p-2) is composite and
 * for every integer r with 5 ≤ r < p-2,   (p-2) ≠ r (mod T(r))
 * where T(r) = r(r+1)/2 is the r‑th triangular number.
 * The function builds the sequence lazily and memoises results
 * to handle out‑of‑order calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility headers ---------------------------------------------------------- */
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */
#include "../utils/is_prime_big.h" /* is_prime_big(seq_t) */
#include "../utils/seq_alloc_new_array.h" /* seq_alloc_new_array(size_t) */
#include "../utils/seq_realloc.h"           /* seq_realloc(seq_t*,size_t,size_t) */

/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* static state for memoisation ---------------------------------------- */
    static idx_t  found = 0;      /* number of terms already found      */
    static idx_t  last = 7;       /* next odd candidate (≡ 1 mod 6)     */
    static seq_t *cache = NULL;   /* array of found primes              */
    static size_t cache_cap = 0;  /* capacity of the cache              */

    /* Allocate or grow cache as needed ------------------------------------- */
    if (n > found) {
        size_t need = n;
        if (cache_cap < need) {
            size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
            if (new_cap < need) new_cap = need;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            if (!tmp) abort();            /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Search consecutive candidates until the n‑th term is found ----- */
        for (;;) {
            if (found >= n) break; /* safety guard */

            seq_t p = (seq_t)last;
            if (!is_prime_big(p)) {
                last += 6;
                continue;
            }

            seq_t m = p - (seq_t)2;
            if (is_prime_big(m)) { /* must be composite */
                last += 6;
                continue;
            }

            /* Compute rmax = floor((√(8m+1)-1)/2) */
            seq_t val   = (m << 3) + (seq_t)1;   /* 8*m + 1 */
            seq_t sqrtv = floor_sqrt(val);
            seq_t rmax  = (sqrtv - (seq_t)1) >> 1; /* divide by 2 */

            /* Convert rmax to idx_t for loop bounds (safe for n<10000) */
            idx_t rmax_i = (idx_t)rmax;

            /* Check the congruence condition for all r in [5, rmax] */
            bool ok = true;
            for (idx_t r = 5; r <= rmax_i; ++r) {
                seq_t tri = ((seq_t)r * (r + (seq_t)1)) / (seq_t)2;
                if ((m % tri) == (seq_t)r) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                cache[found++] = p;            /* store prime */
            }
            last += 6; /* move to next candidate ≡ 1 (mod 6) */
        }
    }

    return cache[n - 1];
}
