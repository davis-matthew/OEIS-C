/*
 * A329401  Numbers whose binary expansion without the most significant
 *          (first) digit is a co-Lyndon word.
 * https://oeis.org/A329401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer whose binary expansion without the *
 *            most significant (first) digit is a co‑Lyndon word. The sequence
 *            * starts 2,3,6,12,14,… as in OEIS A329401
 * Range:     u8 n <= 41, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_co_lyndon
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_co_lyndon.h"

/* 64‑bit floor(log2) for unsigned integers.  */
static inline uint32_t floor_log2_u64(uint64_t v)
{
    uint32_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

/*  A statically sized cache – large enough for all requested indices
 *  in the typical test range (up to about 10000).  The cache is filled
 *  lazily as needed.  The array type seq_t is provided by the build.  */
#define MAX_CACHE 200000
static seq_t seq_cache[MAX_CACHE];
static size_t seq_cnt = 0;          /* number of terms already stored */
static uint64_t next_to_check = 2;  /* next integer to test for property */

/*  Extend the cache until it contains at least n terms.  */
static void extend_cache(idx_t n)
{
    if (seq_cnt >= (size_t)n) return;   /* already enough terms */

    while (seq_cnt < (size_t)n) {
        uint64_t x = next_to_check++;
        uint32_t len = floor_log2_u64(x);          /* number of bits – 1 */
        uint64_t suffix = x & (((uint64_t)1 << len) - 1ULL);
        if (is_co_lyndon(suffix, len)) {
            seq_cache[seq_cnt++] = (seq_t)x;
        }
    }
}

/*  Public sequence function – may be called in any order.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined; return 0 for safety */
    extend_cache(n);
    return seq_cache[(size_t)n - 1];
}
