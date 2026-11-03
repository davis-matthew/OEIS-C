/*
 * A280646  Numbers k such that k^3 has an odd number of digits and the middle
 *          digit is 6.
 * https://oeis.org/A280646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 90, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9728 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A280646: numbers k such that k^3 has an odd
 * number of digits and the middle digit is 6.  There is no known closed
 * form, so the function searches by brute force and memoises the
 * results. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t sz = 0;
    static idx_t cur = 1; /* last candidate checked (k value) */

    /* Ensure the cache contains at least n terms. */
    while (sz < (size_t)n) {
        idx_t k;
        seq_t cube, mid;
        unsigned len, half;

        /* Find the next integer k that satisfies the condition. */
        for (;;) {
            ++cur;
            k = cur;
            seq_t kk = (seq_t)k;
            cube = kk * kk * kk;

            len = (unsigned)num_digits(cube);
            if ((len & 1U) == 0)          /* need odd length */
                continue;

            half = (len - 1U) / 2U;       /* position of middle digit */
            mid = (cube / pow10((idx_t)half)) % (seq_t)10;
            if (mid == (seq_t)6)
                break;
        }

        /* Grow cache if necessary. */
        if (sz == cap) {
            size_t new_cap = (cap == 0 ? 8 : cap << 1);
            seq_t *p = seq_realloc(cache, cap, new_cap);
            cache = p;
            cap = new_cap;
        }

        cache[sz++] = (seq_t)k;
    }

    return cache[(size_t)n - 1];
}
