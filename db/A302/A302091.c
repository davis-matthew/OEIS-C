/*
 * A302091  Primes of form 6*k^k + 5.
 * https://oeis.org/A302091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*k^k + 5 where k is the smallest integer such that 6*k^k
 *            + 5 is a prime; a(1) is the first such prime. This implements
 *            OEIS sequence A302091 with offset 1
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   ipow, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
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

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

static seq_t   *cache     = NULL;   /* dynamic array of primes found so far */
static size_t  cache_len  = 0;      /* number of primes stored in cache   */
static size_t  cache_cap  = 0;      /* allocated capacity of cache       */
static uint64_t k_next     = 1;      /* next k to test */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for offset 0; return 0 to avoid UB. */
        return (seq_t)0;
    }

    /* Extend cache until it contains at least n primes. */
    while (cache_len < (size_t)n) {
        /* Find next prime of the form 6*k^k + 5. */
        for (;;) {
            seq_t base = (seq_t)k_next;
            seq_t exp  = (seq_t)k_next;

            seq_t val = ipow(base, exp);      /* k^k          */
            val = mul_small(val, 6);              /* 6*k^k        */
            val = val + (seq_t)5;                 /* 6*k^k + 5    */

            if (is_prime_big(val)) {
                /* Grow cache if needed and store the new prime. */
                grow_seq_array(&cache, &cache_cap, cache_len + 1);
                cache[cache_len] = val;
                cache_len++;

                /* Advance k_next for next search. */
                k_next++;
                break; /* found a prime, move on to the next one. */
            }
            /* Not prime, test the next k. */
            k_next++;
        }
    }

    return cache[n - 1];
}
