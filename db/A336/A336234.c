/*
 * A336234  Edge length of 'Prime squares': sum the four numbers at the
 *          corners of a square drawn on a diagonally numbered 2D board, with
 *          1 at the corner of the square. The sequence gives the size of the
 *          square such that the sum is a prime number.
 * https://oeis.org/A336234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th odd integer d such that 3*d*d + 4*d + 4 is a
 *            prime
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 80 terms through n = 1915 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A336234.  See the above comment for the
   exact definition.  The function is memoised and works for any
   sequence value that fits in seq_t; it does not assume increasing
   order of calls.  */

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

/* Static cache of computed d values. */
static seq_t *cache = NULL;      /* pointer to cached values */
static size_t cache_cap = 0;     /* current allocated capacity */
static size_t cache_len = 0;     /* number of values stored */
static idx_t next_d = 1;         /* next side length to test */

/* Ensure the cache can hold at least 'need' values. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Find the next d such that 3*d^2+4*d+4 is prime, store it, and
   advance next_d. */
static void extend_cache(void)
{
    for (;;) {
        unsigned long long d = next_d;
        unsigned long long sum = 3ULL * d * d + 4ULL * d + 4ULL;
        seq_t s = (seq_t)sum;
        if (is_prime_big(s)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_d;
            next_d++;                 /* advance to next candidate */
            break;
        }
        next_d++;                     /* not prime – try next d */
    }
}

seq_t a(idx_t n)
{
    /* Ensure we have computed at least n terms. */
    while (cache_len < (size_t)n)
        extend_cache();
    return cache[n - 1];   /* sequence is 1‑based indexed */
}
