/*
 * A280642  Numbers k such that k^3 has an odd number of digits and the middle
 *          digit is 2.
 * https://oeis.org/A280642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 1985, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9120 (2026-11-03)
 * Helpers:   ipow, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates the OEIS sequence A280642:
 * the sequence of integers k such that k^3 has an odd number of
 * decimal digits and the middle digit of that cube is 2.
 * The implementation performs a simple linear search, caching
 * discovered terms so that subsequent calls with any index
 * are answered in O(1) time after that search step.
 * The search continues from the last examined k, so repeated
 * calls are efficient even if the indices are random.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static seq_t *cache   = NULL;   /* cached terms of the sequence */
static size_t  cap     = 0;     /* capacity of cache array */
static size_t  len     = 0;     /* number of terms stored in cache */
static idx_t   curr_k  = 1;     /* next candidate k to test */

/* Ensure the cache has storage for at least `need` elements. */
static void ensure_capacity(size_t need)
{
    if (cap >= need) return;
    size_t newcap = cap ? cap * 2 : 4;
    while (newcap < need) newcap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, newcap);
    if (!tmp) exit(EXIT_FAILURE); /* abort on allocation failure */
    cache = tmp;
    cap = newcap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset is 1, undefined below 1 */

    /* Generate terms until we have at least n terms cached. */
    while (len < (size_t)n) {
        seq_t k = (seq_t)curr_k;
        seq_t cube = ipow(k, 3);          /* k^3 */
        int dlen = num_digits(cube);          /* number of decimal digits */

        if ((dlen & 1) &&                /* odd length? */
            ((cube / pow10(dlen / 2)) % 10) == (seq_t)2) {
            /* this k satisfies the property, cache it */
            ensure_capacity(len + 1);
            cache[len++] = k;
        }
        curr_k++;
    }

    return cache[(size_t)(n - 1)];
}
