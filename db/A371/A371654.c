/*
 * A371654  Numbers that are not multiples of 10 and that yield a prime if
 *          their digits are arranged in ascending order.
 * https://oeis.org/A371654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 9177, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9091 (2026-11-03)
 * Helpers:   digit_counts, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by scanning each integer, skipping multiples of 10, and testing whether
 * the integer formed by its digits in ascending order is prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_counts.h"
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

/* Return the n-th term of sequence A371654 (1‑based index). */
seq_t a(idx_t n)
{
    /* Static cache to allow repeated calls in any order. */
    static seq_t *cache = NULL;           /* array of found terms          */
    static size_t len   = 0;              /* number of terms stored        */
    static size_t cap   = 0;              /* allocated capacity            */
    static seq_t last_checked = 1;        /* largest integer already tested */

    /* Ensure the cache contains at least n terms. */
    if (len < (size_t)n) {
        /* Reallocate cache if necessary. */
        if (cap == 0) {
            cap = 8;
            cache = mem_new(seq_t, cap);
        } else if (len == cap) {
            cap <<= 1;
            cache = mem_resize(seq_t, cache, cap);
        }

        /* Generate terms until we have n of them. */
        seq_t cand = last_checked + 1;
        while (len < (size_t)n) {
            /* Skip multiples of 10. */
            if ((cand % 10) != 0) {
                /* Build the number formed by the digits of cand in ascending order. */
                unsigned char cnt[10];
                digit_counts(cand, cnt);
                seq_t asc = 0;
                for (int d = 0; d < 10; ++d) {
                    for (int i = 0; i < cnt[d]; ++i) {
                        asc = asc * 10 + d;
                    }
                }
                if (is_prime_big(asc)) {
                    /* Term found; store it. */
                    if (len == cap) {
                        cap <<= 1;
                        cache = mem_resize(seq_t, cache, cap);
                    }
                    cache[len++] = cand;
                }
            }
            cand++;
            last_checked = cand - 1;
        }
    }
    return cache[n - 1];
}
