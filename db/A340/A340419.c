/*
 * A340419  Trajectory of 397 under the map A340008: n -> n/2 if n is even,
 *          n-> n^2 - 1 if n is an odd prime, otherwise n -> n - 1.
 * https://oeis.org/A340419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 0, u32 n <= 75, u64 n <= 113, u128 n <= 125,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 3788 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Trajectory of 397 under the map
   a(0) = 397
   a(n+1) = a(n)/2   if a(n) is even
            a(n)^2-1 if a(n) is odd prime
            a(n)-1   if a(n) is odd composite
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

static seq_t *cache = NULL;   /* cached sequence values */
static size_t cap = 0;        /* current allocated capacity */
static idx_t sz = 0;          /* number of terms stored */

/* Ensure the cache is initialized with the initial term a(0)=397 */
static void init_cache(void)
{
    if (!cache) {
        cap = 16;
        cache = mem_new(seq_t, cap);
        if (!cache) exit(1);                     /* allocation failed */
        cache[0] = (seq_t)397;
        sz = 1;
    }
}

/* Return the nth term of the sequence, computing and memoizing as needed */
seq_t a(idx_t n)
{
    init_cache();

    /* If the requested term is not yet computed, extend the cache and
       fill in the missing values. */
    if (n >= sz) {
        size_t needed = (size_t)n + 1;
        if (needed > cap) {
            size_t newcap = cap;
            while (newcap < needed) newcap *= 2;
            seq_t *tmp = mem_resize(seq_t, cache, newcap);
            if (!tmp) exit(1);                 /* allocation failed */
            cache = tmp;
            cap   = newcap;
        }

        /* Compute terms from the current size up to n */
        for (idx_t i = sz; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t next;
            if ((prev % 2) == 0) {            /* even */
                next = prev / 2;
            } else {                         /* odd */
                if (is_prime_big(prev)) {    /* odd prime */
                    next = prev * prev - 1;
                } else {                     /* odd composite */
                    next = prev - 1;
                }
            }
            cache[i] = next;
        }
        sz = n + 1;
    }

    return cache[n];
}
