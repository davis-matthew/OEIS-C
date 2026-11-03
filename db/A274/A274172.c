/*
 * A274172  Nonsquare composites with all prime factors larger than 5.
 * https://oeis.org/A274172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th nonsquare composite that is not divisible by
 *            2,3,5 (equivalent to a composite number whose prime factors are
 *            all > 5)
 * Range:     u8 n <= 13, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9101 (2026-11-03)
 * Helpers:   is_prime_seq, isqrt_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
#include "../utils/isqrt_seq.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but harmless */

    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static seq_t cur = 6;        /* value to be tested next */
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
    }

    /* Generate terms until we have at least n */
    while (terms_len < n) {
        /* Skip numbers divisible by 2, 3 or 5 */
        if ((cur % 2 != 0) && (cur % 3 != 0) && (cur % 5 != 0)) {
            /* Must be composite */
            if (!is_prime_seq(cur)) {
                /* Must not be a perfect square */
                seq_t r = isqrt_seq(cur);
                if (!(r * r == cur)) {
                    /* Store the term */
                    if (terms_cap <= terms_len) {
                        size_t new_cap = terms_cap ? terms_cap * 2 : 32;
                        while (new_cap <= terms_len) new_cap *= 2;
                        grow_seq(&terms, &terms_cap, new_cap);
                    }
                    terms[terms_len++] = cur;
                }
            }
        }
        cur++;   /* advance to next value to examine */
    }

    return terms[n - 1];
}
