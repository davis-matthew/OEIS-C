/*
 * A332785  Nonsquarefree numbers that are not squareful.
 * https://oeis.org/A332785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9510 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A332785: Nonsquarefree numbers that are not
 * squareful.
 * a(n) is the n‑th integer that has at least one prime
 * squared factor (i.e. is nonsquare‑free) but also has
 * at least one prime factor with exponent exactly 1
 * (i.e. is not squareful).  The numbers are generated
 * by testing integers in increasing order and checking
 * the prime factorisation for the required pattern. */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdbool.h>  /* bool */
#include <stdlib.h>   /* malloc, realloc, free */
#include "../utils/mem.h"

/* --- Test whether a number is nonsquare‑free but not squareful -----------*/
static bool is_nonn_squareful(idx_t n)
{
    idx_t tmp = n;
    bool has_sq  = false;
    bool has_one = false;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            if (cnt >= 2) has_sq = true;
            else if (cnt == 1) has_one = true;
        }
    }

    /* any remaining factor > 1 appears exactly once */
    if (tmp > 1) has_one = true;

    return has_sq && has_one;
}

/* --- Integer square‑root for 64‑bit values --------------------------------*/
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    /* simple linear increase is sufficient for the small values
     * we handle (terms < few millions).  For larger values a
     * binary search would be preferable. */
    while ((r + 1) * (r + 1) <= n) r++;
    return r;
}

/* --- Sequence storage ---------------------------------------------------*/
static seq_t *terms      = NULL;      /* array of computed terms   */
static size_t terms_len  = 0;         /* number of terms stored   */
static size_t terms_cap  = 0;         /* allocated capacity       */
static idx_t  next_candidate = 2;      /* next integer to test     */

/* --- Main function -------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (terms == NULL) {            /* first call: initialise */
        terms_cap = 128;
        terms     = mem_new(seq_t, terms_cap);
        terms_len = 0;
        next_candidate = 2;
    }

    while (terms_len < (size_t)n) {
        if (is_nonn_squareful(next_candidate)) {
            if (terms_len == terms_cap) {
                terms_cap <<= 1;     /* double capacity */
                terms = mem_resize(seq_t, terms, terms_cap);
            }
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return terms[(size_t)n - 1];
}
