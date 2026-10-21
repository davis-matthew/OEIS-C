/*
 * A272676  Smallest number greater than previous term such that a(n)^2 begins
 *          with n.
 * https://oeis.org/A272676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 479, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest number greater than the previous term with a(n)^2 beginning with n
   (OEIS A272676).  The function works with any seq_t/idx_t pair; it uses a
   cache to allow arbitrary call order. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/digit_len.h"   /* int digit_len(seq_t x)          */
#include "../utils/pow10.h"      /* seq_t pow10(idx_t e)           */

/* integer decimal digit count for unsigned 64‑bit indices */
static int int_digits(idx_t x) {
    int d = 1;
    while (x >= 10) { d++; x /= 10; }
    return d;
}

seq_t a(idx_t n) {
    /* static cache shared by all calls */
    static seq_t *cache = NULL;
    static size_t len   = 0;
    static size_t cap   = 0;

    /* initialise cache on first use */
    if (!len) {
        cap = 64;                            /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 0;                        /* a(0) = 0       */
        len = 1;
    }

    /* extend cache if necessary */
    while (len <= n) {
        if (len == cap) {
            cap <<= 1;                       /* double capacity */
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }

        seq_t candidate;
        if (len == 0)
            candidate = 0;
        else
            candidate = cache[len - 1] + 1;   /* next integer after previous term */

        /* search for first candidate whose square starts with len */
        for (;;) {
            seq_t sq = candidate * candidate;

            int digits_sq = digit_len(sq);
            int digits_n  = int_digits(len);

            if (digits_sq >= digits_n) {
                idx_t exp  = (idx_t)(digits_sq - digits_n);
                seq_t pow  = pow10(exp);
                seq_t prefix = sq / pow;

                if (prefix == (seq_t)len)
                    break;                    /* found the term */
            }
            candidate += 1;
        }

        cache[len++] = candidate;
    }

    return cache[n];
}
