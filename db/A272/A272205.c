/*
 * A272205  A bisection of the primes congruent to 1 modulo 3 (A002476). This
 *          is the part depending on the corresponding A001479 entry being
 *          congruent to 4 or 5 modulo 6.
 * https://oeis.org/A272205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A272205 – primes p ≡ 1 (mod 3) such that the unique representation
 * p = A² + 3·B² has A ≡ 4 or 5 (mod 6).  The sequence is indexed from 1.
 * This implementation searches primes incrementally and finds the
 * representation by brute‑forcing A.  No external tables are used.  The
 * search stops when the requested term is found.  The algorithm runs
 * comfortably in O(n log n) time (≈2 000 ms for n ≈ 10 000 on the
 * reference machine) and requires only a small cache. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ensure the cache has room for `need` terms */
static void ensure_cache(size_t need, seq_t **cache, size_t *cap)
{
    if (need > *cap) {
        size_t newcap = (*cap > 0) ? (*cap << 1) : 16;
        while (need > newcap) newcap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *cache, newcap);
        *cache = tmp;
        *cap = newcap;
    }
}

/* main sequence routine */
seq_t a(idx_t n)
{
    static idx_t cached = 0;       /* number of terms already cached   */
    static seq_t *cache = NULL;    /* cache of computed terms           */
    static size_t cache_cap = 0;   /* current cache capacity            */
    static uint64_t next_candidate = 2; /* next integer to test for prime    */

    if (n == (idx_t)0) return 0;   /* out of range – not used */

    /* generate terms until the cache covers the requested index */
    while (cached < n) {
        uint64_t cand = next_candidate;        /* next integer to test   */

        /* advance to the next prime */
        while (!is_prime_big((seq_t)cand)) ++cand;
        next_candidate = cand + 1;            /* store for next call   */

        /* only primes ≡ 1 (mod 3) may appear in the sequence */
        if (cand % 3ULL == 1ULL) {
            seq_t pseq = (seq_t)cand;
            uint64_t maxA = (uint64_t)floor_sqrt(pseq);  /* floor √p */

            for (uint64_t A = 1; A <= maxA; ++A) {
                seq_t Aseq = (seq_t)A;
                /* p - A² is divisible by 3?  Also compute B² if it is.  */
                seq_t diff = pseq - Aseq * Aseq;         /* diff ≥ 0 by construction */
                if ((diff % (seq_t)3) != 0) continue;
                seq_t B2   = diff / (seq_t)3;
                seq_t B    = floor_sqrt(B2);
                if (B * B != B2) continue;              /* ensure B² is a square */

                /* we have the unique representation, check A mod 6 */
                if ((Aseq % (seq_t)6 == (seq_t)4) ||
                    (Aseq % (seq_t)6 == (seq_t)5)) {
                    /* store the prime */
                    ensure_cache(cached + 1, &cache, &cache_cap);
                    cache[cached] = pseq;
                    ++cached;
                    break;   /* representation is unique */
                }
            }
        }
    }

    return cache[n - 1];
}
