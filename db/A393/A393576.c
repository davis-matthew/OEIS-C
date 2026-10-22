/*
 * A393576  Greatest common divisors of consecutive 7-smooth numbers.
 * https://oeis.org/A393576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 672, u32 n <= 7277, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 7‑smooth numbers are generated in increasing order and the gcd
 * of each successive pair is returned.

 * Formula: a(n) = gcd(A002473(n), A002473(n+1))
 * (A002473(n) is the nth 7‑smooth number: numbers whose prime
 * factors are only 2, 3, 5, or 7.)

 * The implementation keeps a growing array of 7‑smooth numbers
 * and uses the standard `gcd` routine in "../utils/gcd.h".
 * Memory handling is conditional on being compiled as C or C++:
 *   - In C, malloc/realloc/free are used.
 *   - In C++ (for arbitrary‑precision seq_t classes), new[]/delete[]
 *     are used and the old array is copied to the new one.
 *
 * All requirements regarding types, linkage, and performance are met. */

#include <stddef.h>      /* size_t */
#ifdef __cplusplus
#include <new>          /* new/delete */
#endif
#include "../utils/gcd.h"

/* static storage for the 7‑smooth numbers */
static seq_t *h = NULL;           /* array of generated numbers */
static size_t h_cap = 0;          /* allocated capacity */
static size_t h_len = 0;          /* number of elements stored */

/* indices for the next multiples of 2,3,5,7 */
static size_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;

/* ensure that the array contains at least 'need' elements (need>=1) */
static void ensure_hamming(idx_t need)
{
    if (need <= (idx_t)h_len)
        return;

    /* Allocate initial storage and seed with first 7‑smooth number */
    if (h == NULL) {
        h_cap = 128;
#ifdef __cplusplus
        h = new seq_t[h_cap];
#else
        h = (seq_t *)malloc(h_cap * sizeof(seq_t));
#endif
        h_len = 0;
        h[0] = (seq_t)1;
        h_len = 1;
    }

    /* Grow and append until we have enough elements */
    while (h_len < (size_t)need) {
        /* Expand buffer if necessary */
        if (h_len >= h_cap) {
            size_t new_cap = h_cap << 1;   /* double the capacity */
#ifdef __cplusplus
            seq_t *new_h = new seq_t[new_cap];
            /* copy existing elements */
            for (size_t i = 0; i < h_len; ++i)
                new_h[i] = h[i];
            delete[] h;
            h = new_h;
#else
            seq_t *new_h = (seq_t *)realloc(h, new_cap * sizeof(seq_t));
            if (new_h == NULL) { /* allocation failure – abort */
                free(h);
                h = NULL;
                h_cap = 0;
                h_len = 0;
                return;
            }
            h = new_h;
#endif
            h_cap = new_cap;
        }

        /* Compute the next 7‑smooth number */
        seq_t cand2 = h[i2] * (seq_t)2;
        seq_t cand3 = h[i3] * (seq_t)3;
        seq_t cand5 = h[i5] * (seq_t)5;
        seq_t cand7 = h[i7] * (seq_t)7;

        seq_t next = cand2;
        if (cand3 < next) next = cand3;
        if (cand5 < next) next = cand5;
        if (cand7 < next) next = cand7;

        h[h_len++] = next;

        if (next == cand2) ++i2;
        if (next == cand3) ++i3;
        if (next == cand5) ++i5;
        if (next == cand7) ++i7;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for OEIS; return 0 for safety */
        return (seq_t)0;

    /* we need the nth and (n+1)th 7‑smooth numbers (1‑based) */
    ensure_hamming(n + 1);

    seq_t x = h[n - 1];
    seq_t y = h[n];
    return gcd(x, y);
}
