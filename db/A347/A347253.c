/*
 * A347253  Positive integers that are the product of two integers ending with
 *          4.
 * https://oeis.org/A347253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest positive integers that can be written as * a * b
 *            with a % 10 == 4 and b % 10 == 4 . * The sequence is generated
 *            by brute‑force search with memoisation
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root: floor(sqrt(x)) using only integer operations. */
static inline idx_t isqrt(idx_t x)
{
    idx_t r = 0;
    idx_t bit = 1ULL << 62;          /* highest power of 4 ≤ x for 64‑bit */
    while (bit > x)
        bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

static seq_t *cache      = NULL;   /* stored sequence values   */
static size_t cache_len  = 0;       /* number of values stored  */
static size_t cache_cap  = 0;       /* capacity of stored array */
static idx_t next_num    = 1;       /* next integer to test     */

static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;                           /* already have enough      */

    while (cache_len < n) {
        /* Only numbers ending in 6 can be products of two ending‑4
         * numbers: 4·4 = 16, so the product ends in 6.         */
        while (next_num % 10 != 6)
            ++next_num;

        bool found = false;
        idx_t root = isqrt(next_num);
        for (idx_t a = 4; a <= root; a += 10) {
            if (next_num % a == 0) {
                idx_t b = next_num / a;
                if (b % 10 == 4) {
                    found = true;
                    break;
                }
            }
        }

        if (found) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failure – abort */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_num;
        }
        ++next_num;   /* advance to next candidate */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for 0, but safe     */
    ensure_cache(n);
    return cache[n - 1];
}
