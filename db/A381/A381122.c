/*
 * A381122  Numbers k such that k^(k+1) == k (mod k+2).
 * https://oeis.org/A381122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 2408, u32 n <= 3781, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A381122: numbers k such that k^(k+1) ≡ k (mod k+2).
   The implementation tests integers sequentially, computing k^(k+1) mod (k+2)
   via binary exponentiation (mod_pow). */

#include <stdlib.h>
#include <stdint.h>

/* Binary exponentiation: base^exp mod mod, all operands are of type seq_t
   (which may be an arbitrary‑precision class).  exp is 64‑bit because the
   candidates k are indexed by uint64_t. */
static inline seq_t mod_pow(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1 % mod;          /* 1 mod m */
    seq_t b = base % mod;            /* base mod m (base < mod) */

    while (exp) {
        if (exp & 1ULL)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* Cached sequence values.  The static variables are initialized on the
   first call to a().  The cache grows as needed; it is independent of
   the calling order. */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of terms currently stored */
static size_t cache_cap   = 0;   /* allocated capacity */
static idx_t next_k = 0;        /* next integer k to test (0‑based) */

/* Return the n-th term (1‑based index) of A381122. */
seq_t a(idx_t n)
{
    /* Allocate cache on first call. */
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
    }

    /* Generate until we have at least n terms. */
    while (cache_size < n) {
        seq_t val;
        /* Candidate k = next_k */
        idx_t k = next_k++;

        /* Congruence test: k^(k+1) ≡ k (mod k+2) */
        if (k == 0) {               /* 0^(1) ≡ 0 (mod 2) */
            val = (seq_t)0;
        } else {
            seq_t mod_val = (seq_t)(k + 2);
            seq_t pow_val = mod_pow((seq_t)k, k + 1, mod_val);
            if (pow_val == (seq_t)k)
                val = (seq_t)k;
            else
                continue;          /* not a term; proceed to next candidate */
        }

        /* Grow cache if necessary. */
        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_size++] = val;
    }

    return cache[n - 1];
}
