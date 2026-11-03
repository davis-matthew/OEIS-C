/*
 * A330248  a(1) = 1; for n > 1, a(n) is the least nonnegative number such
 *          that a(n) + a(n-1) + n is a prime number.
 * https://oeis.org/A330248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n) is the smallest non‑negative integer
   such that a(n)+a(n‑1)+n is prime.  The sequence is computed on‑demand
   and cached for future calls.  See OEIS A330248. */

/* Required helper for primality testing */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* Return the nth term of A330248 (1‑based index). */
seq_t a(idx_t n)
{
    /* Special case for the first term. */
    if (n == 1)
        return (seq_t)1;

    /* Static cache for computed terms. */
    static seq_t *cache   = NULL;
    static size_t  size   = 0;   /* number of elements stored (>=1) */
    static size_t  cap    = 0;   /* capacity of cache array          */

    /* Initialise cache on first use. */
    if (cache == NULL) {
        cap = 4;                                 /* initial capacity   */
        cache = mem_new(seq_t, cap);
        if (!cache)                                 /* allocation failed  */
            return 0;
        cache[0] = (seq_t)1;                       /* a(1) */
        cache[1] = (seq_t)0;                       /* a(2) */
        size    = 2;                               /* two terms stored   */
    }

    /* Grow cache if required. */
    if ((size_t)n > cap) {
        while (cap < (size_t)n)
            cap <<= 1;                         /* double the size */
        cache = mem_resize(seq_t, cache, cap);
        if (!cache)                                /* allocation failed  */
            return 0;
    }

    /* Return cached value if already computed. */
    if ((size_t)n <= size)
        return cache[n - 1];

    /* Compute missing terms up to n. */
    for (idx_t k = (idx_t)(size + 1); k <= n; ++k) {
        seq_t prev     = cache[k - 2];         /* a(k-1)            */
        seq_t candidate = 0;                   /* a(k) candidate   */
        /* Find the smallest non‑negative candidate. */
        while (1) {
            seq_t sum = candidate + prev + (seq_t)k;
            if (is_prime_big(sum))
                break;
            ++candidate;
        }
        cache[k - 1] = candidate;              /* store a(k)        */
    }

    size = n;
    return cache[n - 1];
}
