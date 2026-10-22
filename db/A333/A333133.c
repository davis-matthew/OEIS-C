/*
 * A333133  7-Kaprekar numbers.
 * https://oeis.org/A333133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 7-Kaprekar numbers: a(n) is the nth integer k such that
   k = floor(k² / 10⁷) + (k² mod 10⁷).  */
#include <stdlib.h>
#include <stddef.h>

#define KAPREKAR_DIGITS 7
#define KAPREKAR_MOD     10000000U
#define MAX_TERM_CACHE   20000U   /* enough for n up to 10000 */

seq_t a(idx_t n)
{
    static seq_t mod = 0;
    if (mod == 0) mod = KAPREKAR_MOD;          /* 10⁷ */

    static seq_t current = 0;                  /* last tested value   */
    static size_t cache_len = 0;               /* number of terms found */
    static seq_t cache[MAX_TERM_CACHE];        /* cached terms */

    /* generate terms until the n‑th one is available */
    while ((size_t)n > cache_len) {
        while (1) {
            ++current;
            seq_t sq = current * current;
            seq_t part2 = sq % mod;            /* right 7 digits   */
            seq_t part1 = sq / mod;            /* left part        */
            if (part1 + part2 == current) {    /* Kaprekar test   */
                if (cache_len == MAX_TERM_CACHE) abort(); /* safety */
                cache[cache_len++] = current;
                break;
            }
        }
    }

    return cache[(size_t)n - 1];                /* a(1) is stored at index 0 */
}
