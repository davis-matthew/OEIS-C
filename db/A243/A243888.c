/*
 * A243888  Primes of the form 2*n^2+26*n+11.
 * https://oeis.org/A243888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 26*n + 11 for that n where the value is prime
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 994 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates 2*n²+26*n+11 and keeps all primes found in a static
   cache, so repeated calls for arbitrary n <=> O(n) total operations. */

#include <stdbool.h>
#include <stdlib.h>
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

/* Helper to cast the result of realloc/malloc in a way that compiles both C
   and C++.  In C the cast is implicit; in C++ we must use reinterpret_cast. */
#if defined(__cplusplus)
# define ALLOC_CAST(ptr, type) reinterpret_cast<type>(ptr)
#else
# define ALLOC_CAST(ptr, type) ((type)(ptr))
#endif

seq_t a(idx_t n)
{
    /* static cache keeps primes that have already been found */
    static seq_t *cache      = NULL; /* array of found primes       */
    static size_t  capacity  = 0;    /* current allocated capacity  */
    static size_t  count     = 0;    /* number of stored primes     */
    static idx_t   last_test = 0;    /* last n that was tested      */

    /* generate primes until we have at least n of them */
    while (count < n) {
        /* test next n value */
        ++last_test;

        /* compute 2*n^2 + 26*n + 11 in seq_t arithmetic */
        seq_t sq   = (seq_t)last_test;          /* n                  */
        seq_t val  = sq * sq;                   /* n^2                */
        val        = val * (seq_t)2;            /* 2*n^2              */
        val        = val + (sq * (seq_t)26);    /* +26*n              */
        val        = val + (seq_t)11;           /* +11                */

        if (is_prime_big(val)) {
            /* enlarge cache if necessary */
            if (count == capacity) {
                size_t newcap = capacity ? capacity * 2 : 16;
                seq_t *tmp = ALLOC_CAST(mem_resize(seq_t, cache, newcap), seq_t*);
                if (!tmp) {
                    /* allocation failed – best we can do is abort */
                    exit(EXIT_FAILURE);
                }
                cache    = tmp;
                capacity = newcap;
            }
            cache[count++] = val;
        }
    }

    return cache[n - 1];
}
