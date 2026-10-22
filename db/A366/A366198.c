/*
 * A366198  Any a(n) replacing the first digit of a(n+1) forms a palindrome.
 *          This is the lexicographically earliest sequence of distinct
 *          nonnegative integers with this property.
 * https://oeis.org/A366198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 263, u32 n <= 664, u64 n <= 1468,
 *            u128 n <= 3011, big any n
 * Helpers:   digit_len, is_palindrome, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A366198: a(n) = 10*a(n-81) + 90 - 9*(a(n-81) mod 10) for n ≥ 92; 
   initial terms: a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=4, a(6)=5, a(7)=6,
   a(8)=7, a(9)=8, a(10)=9.  The sequence is built lexicographically
   earliest by searching for the smallest unused nonnegative integer
   that, when its first digit is replaced by a(n‑1), yields a palindrome. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow10.h"        /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h"    /* int digit_len(seq_t x); */
#include "../utils/is_palindrome.h"/* bool is_palindrome(seq_t x, idx_t base); */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* sequence offset is 1 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;               /* allocated size */
    static size_t stored = 0;            /* number of computed terms */

    /* Ensure cache capacity for n terms */
    if ((size_t)n > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure */
        cache = tmp;
        cap = (size_t)n;
    }

    /* initialise first term if not yet done */
    if (stored == 0) {
        cache[0] = (seq_t)0;             /* a(1) = 0 */
        stored = 1;
    }

    /* compute remaining terms up to n */
    for (size_t i = stored; i < (size_t)n; ++i) {
        if (i < 91) {                    /* for n < 92, use search */
            seq_t prev = cache[i - 1];
            seq_t cand = 0;

            for ( ;; ++cand ) {
                /* check that cand has not been used before */
                bool dup = false;
                for (size_t j = 0; j < i; ++j) {
                    if (cache[j] == cand) { dup = true; break; }
                }
                if (dup) continue;

                /* form number obtained by replacing the first digit of cand
                 * by the decimal representation of prev */
                int lenC = digit_len(cand);          /* λ(cand) */
                if (lenC <= 0) lenC = 1;             /* safety */
                seq_t pow10_len = pow10((idx_t)(lenC - 1));
                seq_t suffix   = cand % pow10_len;
                seq_t L        = prev * pow10_len + suffix;

                if (is_palindrome(L, 10)) {
                    cache[i] = cand;
                    break;
                }
            }
        } else {                        /* recurrence for n ≥ 92 */
            seq_t base = cache[i - 81];                 /* a(n-81) */
            seq_t rem  = base % (seq_t)10;
            cache[i] = base * (seq_t)10
                      + (seq_t)90 - (seq_t)9 * rem;
        }
    }

    stored = (size_t)n;
    return cache[n - 1];
}
