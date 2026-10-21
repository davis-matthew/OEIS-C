/*
 * A293657  Base-4 circular primes that are not base-4 repunits.
 * https://oeis.org/A293657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) is obtained by searching all base‑4 numbers whose digits
 *            are only 1 or 3 (excluding the repunit 111…1), testing that all
 *            rotations are prime, and collecting the decimal values in
 *            ascending order. The function caches results so that any call
 *            order is supported
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility from the build environment */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */
#include "../utils/seq_realloc.h"   /* seq_t* seq_realloc(seq_t*, size_t, size_t) */

/* External types supplied by the build */
extern seq_t a(idx_t n);

/* ---------- internal state ---------- */
static seq_t *cache      = NULL;   /* cached terms */
static size_t cacheSize = 0;       /* number of stored terms   */
static size_t cacheCap  = 0;       /* allocated array capacity */
static idx_t  nextLen    = 2;      /* next base‑4 length to test */

/* Append a value to the cache, expanding it if necessary. */
static void append_to_cache(seq_t val)
{
    if (cacheSize >= cacheCap) {
        size_t newCap = (cacheCap == 0) ? 16 : cacheCap * 2;
        seq_t *tmp = seq_realloc(cache, cacheCap, newCap);
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cacheCap = newCap;
    }
    cache[cacheSize++] = val;
}

/* Generate all base‑4 numbers of a given length L with digits 1 or 3,
   test each for the circular‑prime property, and store the valid ones. */
static void generate_len(idx_t L)
{
    /* Decide whether seq_t can hold values up to 64‑bit. */
    const bool small_seq = (sizeof(seq_t) <= 8);
    const uint64_t maxU64 = small_seq ? ~0ULL : 0;

    /* Generate all bit patterns for the L low bits:
       bit = 1 → digit 3, bit = 0 → digit 1.  Skip the all‑ones repunit. */
    const size_t maskStart = 1;                     /* 0b01... with least significant bit = 1   */
    const size_t maskEnd   = ((size_t)1) << L;      /* one past the last mask */

    for (size_t mask = maskStart; mask < maskEnd; ++mask) {
        /* Build the decimal value in base 10. */
        unsigned __int128 val128 = 0;
        for (idx_t i = 0; i < L; ++i) {
            unsigned int d = ((mask >> (L - 1 - i)) & 1) ? 3 : 1;
            val128 = val128 * 4 + d;
            if (small_seq && val128 > maxU64) break;   /* too large for seq_t */
        }
        if (small_seq && val128 > maxU64) continue;

        seq_t val = (seq_t)val128;

        /* Test every rotation for primality. */
        bool ok = true;
        for (idx_t rot = 0; rot < L && ok; ++rot) {
            unsigned __int128 r128 = 0;
            for (idx_t j = 0; j < L; ++j) {
                unsigned int d = ((mask >> ((L - 1 - ((j + rot) % L)))) & 1) ? 3 : 1;
                r128 = r128 * 4 + d;
                if (small_seq && r128 > maxU64) { ok = false; break; }
            }
            if (!ok) break;
            seq_t r = (seq_t)r128;
            if (!is_prime_big(r)) ok = false;
        }
        if (ok) append_to_cache(val);
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                         /* undefined for n<1 */
    while (cacheSize < (size_t)n) {               /* generate until enough terms */
        generate_len(nextLen++);
    }
    return cache[(size_t)n - 1];
}
