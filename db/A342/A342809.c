/*
 * A342809  Numbers k such that k-1 and round(k/5) are both prime.
 * https://oeis.org/A342809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   k such that k-1 and round(k/5) are primes. Computed by
 *            brute‑force search with memoisation
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
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

seq_t a(idx_t n)
{
    /* static cache holding all already found terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* next candidate value of k */
    static seq_t next_k = 1;

    /* Initialise the cache on the first call */
    if (cache_cap == 0) {
        cache_cap = 4;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) exit(1);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* seek next k such that k-1 and round(k/5) are prime */
        while (true) {
            /* Test k-1 */
            if (is_prime_big(next_k - 1)) {
                /* rounded(k/5) = floor((k+2)/5) */
                seq_t r = (next_k + 2) / (seq_t)5;
                if (is_prime_big(r)) {
                    /* cache this term */
                    if (cache_len + 1 > cache_cap) {
                        cache_cap *= 2;
                        seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                        if (!tmp) exit(1);
                        cache = tmp;
                    }
                    cache[cache_len++] = next_k;
                    break;          /* go on to the next term */
                }
            }
            /* try the next integer */
            ++next_k;
        }
        /* advance to the following integer after the one just processed */
        ++next_k;
    }

    return cache[n - 1];
}
