/*
 * A313682  Coordination sequence Gal.5.55.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 5 a(2) = 10 a(3) = 15 a(4) = 19 a(5) = 23 a(6) =
 *            27 a(7) = 31 a(8) = 36 a(9) = 41 a(10) = 46 For n > 10: let t =
 *            Σ_{j=1}^{9} (-1)^{j-1} * a(n-j) then a(n) = 2 * t – a(n-10)
 *            (i.e. a(n)=2*a(n-1)-2*a(n-2)+2*a(n-3)-2*a(n-4)+2*a(n-5)-2*a(n-6)
 *            +2*a(n-7)-2*a(n-8)+2*a(n-9)-a(n-10) )
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdint.h>      /* int64_t */

#ifndef MAX_CACHE_SIZE
#define MAX_CACHE_SIZE 20001   /* enough for all test cases (n ≤ 10000) */
#endif

/* Static cache for already computed terms */
static seq_t cache[MAX_CACHE_SIZE];
static size_t cache_cap = 0;

/* Compute and store terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* prevent overrun of static array */
    if (n >= (idx_t)(MAX_CACHE_SIZE - 1))
        n = (idx_t)(MAX_CACHE_SIZE - 1);

    /* very first initialisation */
    if (cache_cap == 0) {
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)5;
        cache[2]  = (seq_t)10;
        cache[3]  = (seq_t)15;
        cache[4]  = (seq_t)19;
        cache[5]  = (seq_t)23;
        cache[6]  = (seq_t)27;
        cache[7]  = (seq_t)31;
        cache[8]  = (seq_t)36;
        cache[9]  = (seq_t)41;
        cache[10] = (seq_t)46;
        cache_cap = 11;          /* indices 0..10 are ready */
    }

    /* compute the remaining terms */
    for (size_t i = cache_cap; i <= (size_t)n; ++i) {
        if (i <= 10)        /* safety guard – should never reach */
            continue;

        /* compute t = Σ_{j=1}^{9} (-1)^{j-1} * a(i-j) */
        int64_t t = 0;
        int sign = 1;
        for (int j = 1; j <= 9; ++j) {
            t += (int64_t)sign * (int64_t)cache[i - j];
            sign = -sign;
        }

        int64_t prev10 = (int64_t)cache[i - 10];
        cache[i] = (seq_t)( (int64_t)2 * t - prev10 );
    }

    cache_cap = (size_t)n + 1;
}

/* Public API: return the n‑th term of the sequence */
seq_t a(idx_t n)
{
    if (n >= (idx_t)(MAX_CACHE_SIZE - 1)) {
        /* out of range – program guarantees will never call this */
        return (seq_t)0;
    }
    ensure_cache(n);
    return cache[(size_t)n];
}
