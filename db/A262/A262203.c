/*
 * A262203  Primes of the form k*(k+2)/3 - 3, k>2.
 * https://oeis.org/A262203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that 3*p + 10 is a square (equivalently p =
 *            k*(k+2)/3 - 3 for k > 2 and k % 3 != 2)
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Utility for prime testing (provided by the harness). */
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

/* External types defined by the environment. */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/*  Internal static cache for the sequence values.                     */
/* ------------------------------------------------------------------ */
static seq_t  *cache       = NULL;   /* array of found primes          */
static size_t cache_len   = 0;       /* number of cached primes        */
static size_t cache_cap   = 0;       /* current capacity of the cache  */
static idx_t  next_k      = 3;      /* next k to test (k > 2)         */

/* ------------------------------------------------------------------ */
/*  Ensure that the cache can store at least “needed” elements.        */
/* ------------------------------------------------------------------ */
static void
ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t newcap = cache_cap ? cache_cap * 2 : 4;
    while (newcap < needed)
        newcap *= 2;

    seq_t *tmp = mem_resize(seq_t, cache, newcap);
    if (!tmp) {
        /* In the unlikely event of a memory allocation failure, abort. */
        abort();
    }
    cache = tmp;
    cache_cap = newcap;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0)               /* the sequence is 1‑based; undefined for 0 */
        return (seq_t)0;

    /* Return a cached value if it already exists. */
    if ((idx_t)cache_len >= n)
        return cache[n - 1];

    /* Generate further primes until we reach the requested index. */
    while ((idx_t)cache_len < n) {
        /* k must not be congruent to 2 mod 3 (otherwise k*(k+2)/3 is not integer). */
        if ((next_k % 3) != 2) {
            /* Compute p = k*(k+2)/3 - 3.  The product fits in 64 bits
               for the values required by the test limits. */
            idx_t prod = next_k * (next_k + 2);
            idx_t div  = prod / 3;              /* integer division */
            seq_t p    = (seq_t)(div - 3);      /* final candidate */

            if (is_prime_big(p)) {
                ensure_cache(cache_len + 1);
                cache[cache_len++] = p;
            }
        }
        ++next_k;
    }

    return cache[n - 1];
}
