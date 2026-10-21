/*
 * A261920  In A261907 but not the sum of two palindromes whose lengths differ
 *          by 1.
 * https://oeis.org/A261920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   digit_len, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th element of OEIS sequence A261920:
   Non‑palindromic integers that are representable as a sum of two
   palindromes, but not as a sum of two palindromes whose decimal
   lengths differ by one.  The implementation pre‑computes all
   qualifying numbers up to a fixed maximum value and serves any
   requested index from the cached array. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Maximum value for which sums of two palindromes are pre‑computed.
   200 000 guarantees more than 10 000 valid terms. */
#define MAX_VAL 200000UL

/* Static state for memoisation. */
static bool initialized = false;
static uint8_t *is_pal;        /* 1 if i is palindrome, else 0 */
static uint8_t *has_any;       /* 1 if i = a + b for some palindromes a,b */
static uint8_t *has_diff1;     /* 1 if i = a + b with |len(a)-len(b)| == 1 */
static uint64_t *qual;         /* qualifying numbers (up to MAX_VAL) */
static size_t qual_len = 0;    /* number of qualifying numbers cached */

/* Utility helpers provided by the build. */
#include "../utils/is_palindrome.h"   /* bool is_palindrome(seq_t x, idx_t base) */
#include "../utils/digit_len.h"       /* int digit_len(seq_t x) */

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* First time we are called, initialise the cached data. */
    if (!initialized) {
        /* Allocate flag arrays.  malloc returns void*, therefore explicit cast
           required for C++. */
        is_pal    = (uint8_t *)malloc((MAX_VAL + 1) * sizeof(uint8_t));
        has_any   = (uint8_t *)malloc((MAX_VAL + 1) * sizeof(uint8_t));
        has_diff1 = (uint8_t *)malloc((MAX_VAL + 1) * sizeof(uint8_t));
        if (!is_pal || !has_any || !has_diff1) {
            /* Allocation failure – abort further initialisation. */
            free(is_pal); free(has_any); free(has_diff1);
            initialized = true;
            return (seq_t)0;
        }
        for (size_t i = 0; i <= MAX_VAL; ++i) { is_pal[i] = 0; has_any[i] = 0; has_diff1[i] = 0; }

        /* -----------------------------------------------------------------
           1. Build list of palindromes up to MAX_VAL. */
        size_t pal_cap = 64;
        uint64_t *pal_val = (uint64_t *)malloc(pal_cap * sizeof(uint64_t));
        int *pal_len_arr = (int *)malloc(pal_cap * sizeof(int));
        if (!pal_val || !pal_len_arr) { free(pal_val); free(pal_len_arr); initialized = true; return (seq_t)0; }
        size_t pal_cnt = 0;

        for (uint64_t i = 1; i <= MAX_VAL; ++i) {
            bool pal = is_palindrome((seq_t)i, 10);
            is_pal[(size_t)i] = pal;
            if (pal) {
                if (pal_cnt == pal_cap) {
                    pal_cap <<= 1;
                    pal_val = (uint64_t *)realloc(pal_val, pal_cap * sizeof(uint64_t));
                    pal_len_arr = (int *)realloc(pal_len_arr, pal_cap * sizeof(int));
                    if (!pal_val || !pal_len_arr) { free(pal_val); free(pal_len_arr); initialized = true; return (seq_t)0; }
                }
                pal_val[pal_cnt] = i;
                pal_len_arr[pal_cnt] = digit_len((seq_t)i);
                ++pal_cnt;
            }
        }

        /* -----------------------------------------------------------------
           2. Mark all sums of two palindromes. */
        for (size_t i = 0; i < pal_cnt; ++i) {
            uint64_t p = pal_val[i];
            int lenp   = pal_len_arr[i];
            for (size_t j = i; j < pal_cnt; ++j) {
                uint64_t q = pal_val[j];
                int lenq   = pal_len_arr[j];
                uint64_t s = p + q;
                if (s > MAX_VAL) break;
                has_any[(size_t)s] = 1;
                if ((lenp > lenq ? lenp - lenq : lenq - lenp) == 1)
                    has_diff1[(size_t)s] = 1;
            }
        }

        /* -----------------------------------------------------------------
           3. Collect qualifying numbers. */
        qual = (uint64_t *)malloc((MAX_VAL + 1) * sizeof(uint64_t));
        if (!qual) { free(pal_val); free(pal_len_arr); initialized = true; return (seq_t)0; }
        for (uint64_t s = 10; s <= MAX_VAL; ++s) {
            if (!is_pal[(size_t)s] && has_any[(size_t)s] && !has_diff1[(size_t)s]) {
                qual[qual_len++] = s;
            }
        }

        /* Free temporary lists. */
        free(pal_val);
        free(pal_len_arr);

        initialized = true;
    }

    /* Guard against out‑of‑range requests. */
    if (n < 1 || (idx_t)n > qual_len) return (seq_t)0;

    /* Cast to seq_t – works for plain integer types and for big integer
       classes that have a constructor from uint64_t. */
    return (seq_t)qual[(size_t)n - 1];
}
