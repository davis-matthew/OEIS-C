/*
 * A283806  Odd numbers which are uniquely decomposable into the sum of a
 *          prime and a power of two.
 * https://oeis.org/A283806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 996 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A283806: odd numbers uniquely decomposable as prime + power of two.
   Search‑based implementation that caches discovered terms. */

#include <stddef.h>
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

seq_t a(idx_t n)
{
    /* static cache of discovered terms */
    static seq_t *terms = NULL;
    static size_t len   = 0;
    static size_t cap   = 0;

    /* first valid index is 1 */
    if (n < (idx_t)1)
        return (seq_t)0;

    /* initialise cache */
    if (!terms) {
        cap   = 16;
        terms = mem_new(seq_t, cap);
        if (!terms)
            return (seq_t)0;   /* out of memory -> return 0 */
    }

    /* extend cache until it contains at least n terms */
    while (len < (size_t)n) {
        /* next odd candidate */
        seq_t cand = len == 0 ? (seq_t)3 : terms[len - 1] + (seq_t)2;

        /* find the next valid term */
        while (1) {
            size_t prime_cnt = 0;
            seq_t   pow = (seq_t)1;

            /* iterate over powers of two <= cand */
            for (; pow <= cand; ) {
                seq_t diff = cand - pow;
                if (diff >= (seq_t)2 && is_prime_big(diff))
                    ++prime_cnt;
                if (prime_cnt > 1)
                    break; /* more than one decomposition */

                /* avoid overflow on the next multiplication */
                if (pow > cand / (seq_t)2)
                    break;
                pow *= (seq_t)2;
            }

            if (prime_cnt == 1) {
                /* store the term, grow array if necessary */
                if (len == cap) {
                    size_t new_cap = cap * 2;
                    seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                    if (!tmp)
                        return (seq_t)0;   /* realloc failed */
                    terms = tmp;
                    cap   = new_cap;
                }
                terms[len++] = cand;
                break; /* proceed to next candidate */
            }

            cand += (seq_t)2;   /* try the next odd number */
        }
    }

    return terms[n - 1];
}
