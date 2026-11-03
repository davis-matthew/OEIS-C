/*
 * A248620  Lesser of twin primes of (29n + 1, 29n + 3).
 * https://oeis.org/A248620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer of the form 29k+1 such that 29k+1 and
 *            29k+3 are both prime (the lesser of the twin primes). The
 *            function searches for twin primes of the form 29k+1, 29k+3 and
 *            caches the 29k+1 values
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 54 terms through n = 966 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t *cache     = NULL;   /* cached 29k+1 values */
static size_t cache_len = 0;      /* number of stored terms      */
static size_t cache_cap = 0;      /* allocated capacity          */
static idx_t  last_k    = 1;      /* last k examined (start at 1) */

/*--- dynamic array helpers ----------------------------------------*/
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = (needed > 4) ? needed * 2 : 4;   /* start reasonably */
    seq_t *new_buf = mem_resize(seq_t, cache, new_cap);
    if (!new_buf)               /* allocation failed */
        abort();
    cache = new_buf;
    cache_cap = new_cap;
}

/*--- generate terms up to n_needed --------------------------------*/
static void generate_up_to(idx_t n_needed)
{
    size_t needed = (size_t)n_needed;
    while (cache_len < needed) {
        ++last_k;                            /* try next k          */
        seq_t p  = (seq_t)29 * (seq_t)last_k + (seq_t)1;
        seq_t p2 = p + (seq_t)2;             /* 29k+3               */
        if (is_prime_big(p) && is_prime_big(p2)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = p;          /* store 29k+1         */
        }
    }
}

/*--- public function ----------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)            /* outside offset, return 0 as safe default */
        return (seq_t)0;
    generate_up_to(n);     /* ensure we have at least n terms */
    return cache[(size_t)n - 1];   /* zero‑based array index */
}
