/*
 * A378794  Numbers k such that tau(k) == 1 (mod(2*(tau(k^2 + k - 1)))), where
 *          tau(k) = A000005(k).
 * https://oeis.org/A378794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) consists of the n‑th square k = m² for which
   tau(k) ≡ 1 (mod 2 · tau(k² + k – 1)), where tau(x) counts the
   divisors of x. This implementation searches over successive
   squares m² and caches the qualifying values. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"

/* cache of computed terms */
static seq_t *cache = NULL;
/* number of cached terms */
static size_t len = 0;
/* capacity of the cache array */
static size_t cap = 0;
/* next m (i.e. sqrt(k)) to test */
static idx_t next_m = 1;

/* enlarge the cache if necessary and append a new value */
static void push_back(seq_t val)
{
    if (len == cap) {
        cap = cap ? cap * 2 : 8;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }
    cache[len++] = val;
}

/* return the n‑th term of A378794 (n ≥ 1) */
seq_t a(idx_t n)
{
    while (len < n) {
        idx_t m = next_m++;                     /* current square root */
        seq_t k = (seq_t)m * (seq_t)m;          /* k = m² */

        /* tau(k) – count divisors of m²; use divisor_count_u64 */
        uint64_t tau_k = divisor_count_u64((uint64_t)k);

        /* t = k² + k – 1 = m⁴ + m² – 1; compute in 128‑bit to avoid overflow */
        __int128 m128 = (__int128)m;
        __int128 t128 = m128 * m128 * m128 * m128 + m128 * m128 - 1;
        uint64_t t = (uint64_t)t128;            /* fits in 64 bits for the needed range */

        /* tau(t) */
        uint64_t tau_t = divisor_count_u64(t);

        /* check the congruence condition */
        uint64_t mod = 2 * tau_t;
        if (tau_k % mod == 1) {
            push_back(k);
        }
    }

    return cache[n - 1];
}
