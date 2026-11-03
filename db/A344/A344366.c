/*
 * A344366  Integers k such that the sum of squares of digits of both k and
 *          k-1 are prime.
 * https://oeis.org/A344366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function searches incrementally for integers k where the sum of
   squares of digits of k and of k‑1 are both prime.  It memoizes the
   results in a static dynamic array so that any index can be requested
   in any order. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Helper: sum of squares of decimal digits of a 64‑bit integer. */
static uint64_t digit_square_sum(uint64_t x)
{
    uint64_t sum = 0;
    while (x > 0) {
        uint64_t d = x % 10;
        sum += d * d;
        x /= 10;
    }
    return sum;
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

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term.  Below that the result is
       irrelevant, but a small guard is harmless. */
    if (n < 1)
        return 0;

    /* Static state for memoisation */
    static seq_t *cache   = NULL; /* array of found terms */
    static size_t cap     = 0;    /* current capacity of the array */
    static size_t len     = 0;    /* number of terms stored */
    static uint64_t next_candidate = 1; /* smallest integer to test next */

    /* Generate terms until we have at least n of them. */
    while (len < n)
    {
        /* Search for the next qualifying k.  We start with the first
           integer strictly larger than the last found candidate. */
        uint64_t k;
        for (k = next_candidate + 1; ; ++k)
        {
            /* Sum of squares of digits for k. */
            uint64_t sum_k = digit_square_sum(k);
            if (!is_prime_big((seq_t)sum_k))
                continue;

            /* Sum of squares of digits for k-1. */
            uint64_t sum_km1 = digit_square_sum(k - 1);
            if (!is_prime_big((seq_t)sum_km1))
                continue;

            /* k satisfies the condition. */
            next_candidate = k;
            break;
        }

        /* Ensure we have enough capacity to store the new term. */
        if (len == cap)
        {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = seq_alloc_new_array(new_cap);
            if (!tmp)
                exit(EXIT_FAILURE);   /* allocation failure */

            /* Copy existing terms to the new array. */
            for (size_t i = 0; i < len; ++i)
                tmp[i] = cache[i];

            /* Free old storage and replace with the new array. */
            if (cache)
                seq_free_array(cache);
            cache = tmp;
            cap   = new_cap;
        }

        /* Store the newly found term. */
        cache[len++] = (seq_t)next_candidate;
    }

    /* Return the requested element. */
    return cache[n - 1];
}
