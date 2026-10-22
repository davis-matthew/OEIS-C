/*
 * A338081  Odd composite integers such that A054413(m)^2 == 1 (mod m).
 * https://oeis.org/A338081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   U_mod_func, is_prime_small, mul_mat, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A338081:
   Odd composite integers m such that U(m)^2 ≡ 1 (mod m)
   where U is the Lucas sequence with parameters a = 7, b = -1:
       U(0) = 0, U(1) = 1,
       U(k+2) = 7·U(k+1) + U(k).
   The function searches odd composite numbers in increasing order
   and memoises them so that any call to a(n) is O(1) after the
   required terms have been generated.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/seq_realloc.h"
#include "../utils/U_mod_func.h"
#include "../utils/mul_mat.h"

/* ---------- memoisation infrastructure ---------- */
static seq_t *cache = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;
static uint64_t next_candidate = 3;   /* odd numbers start at 3 */

static void cache_append(uint64_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) abort();                 /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = (seq_t)val;        /* cast to seq_t */
}

/* ---------- public interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset 1; not used */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        uint64_t m = next_candidate;
        /* odd composite check */
        if ( (m & 1ULL) && !is_prime_small(m) ) {
            uint64_t u = U_mod_func(m, m);
            seq_t u2 = ((seq_t)u * (seq_t)u) % (seq_t)m;   /* U(m)^2 mod m */
            if (u2 == (seq_t)1) {
                cache_append(m);
            }
        }
        next_candidate += 2;   /* next odd number */
    }
    return cache[n-1];        /* 1‑based indexing */
}
