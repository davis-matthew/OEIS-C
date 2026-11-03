/*
 * A321523  List of pairs: primes whose reversal is also prime, each followed
 *          by its reversal.
 * https://oeis.org/A321523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) = the n-th element of the list of all primes p where rev(p)
 *            is prime, followed by rev(p). The sequence is 1‑based
 * Range:     u8 n <= 28, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   is_prime_u64, mem, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utilities provided by the build system. */
#include <stddef.h>
#include "../utils/reverse_digits_idx.h"
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

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void grow_cache(seq_t **cache, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = *cap ? *cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = alloc_seq_array(new_cap);
    if (!tmp) abort();
    *cache = tmp;
    *cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1; safe default */

    /* Static cache of sequence terms, filled lazily. */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0, cache_cap = 0;
    static uint64_t candidate = 0;  /* current candidate to test */

    /* Ensure enough space in the cache. */
    grow_cache(&cache, &cache_cap, (size_t)n);

    /* Generate terms until we have one at index n. */
    while (cache_sz < (size_t)n)
    {
        /* Find next prime candidate. */
        if (candidate == 0) candidate = 2;
        while (!is_prime_big((seq_t)candidate))
        {
            candidate += (candidate == 2 ? 1 : 2);  /* advance to next odd */
        }

        /* Check whether the reversal is also prime. */
        seq_t rev_candidate = reverse_digits_idx(candidate);
        if (is_prime_big(rev_candidate))
        {
            /* Append the pair to the cache. */
            grow_cache(&cache, &cache_cap, cache_sz + 2);
            cache[cache_sz++] = (seq_t)candidate;
            cache[cache_sz++] = rev_candidate;
        }

        /* Advance to the next candidate for the next loop iteration. */
        candidate += (candidate == 2 ? 1 : 2);
    }

    /* Return the 1‑based term. */
    return cache[n - 1];
}
