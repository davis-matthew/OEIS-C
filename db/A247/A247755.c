/*
 * A247755  Numbers in decimal representation, such that in German their
 *          digits are in alphabetic order.
 * https://oeis.org/A247755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 1991, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247755: a(n) is the n‑th natural number whose decimal digits
   are in nondecreasing order of the German alphabetical order of the
   digits’ names.  The algorithm scans the natural numbers, keeps the
   valid ones in a cache, and returns the n‑th element. */

#include <stdbool.h>
#include <stdlib.h>

/* Rank of each digit in German alphabetical order:
   8(acht) < 3(drei) < 1(eins) < 5(fünf) < 9(neun) < 0(null)
   < 6(sechs) < 7(sieben) < 4(vier) < 2(zwei) */
static const int rank_map[10] = {5, 2, 9, 1, 8, 3, 6, 7, 0, 4};

static bool is_valid(seq_t x)
{
    if (x == 0) return true;          /* 0 itself is valid */
    int digits[20];                   /* enough for 64‑bit numbers */
    int cnt = 0;
    seq_t t = x;

    /* extract digits least significant first */
    while (t != 0) {
        digits[cnt++] = (int)(t % 10);
        t /= 10;
    }

    int prev_rank = -1;
    /* walk from most significant to least */
    for (int i = cnt - 1; i >= 0; --i) {
        int d = digits[i];
        int r = rank_map[d];
        if (prev_rank != -1 && r < prev_rank) return false;
        prev_rank = r;
    }
    return true;
}

/* Cache for already computed terms */
static seq_t *cache            = NULL;
static idx_t  cache_count      = 0;
static idx_t  cache_capacity   = 0;
static seq_t  next_scan        = 0;   /* next integer to examine */

seq_t a(idx_t n)
{
    /* Ensure cache contains at least n terms */
    while (cache_count < n) {
        if (is_valid(next_scan)) {
            if (cache_count == cache_capacity) {
                idx_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              new_cap * sizeof(seq_t));
                cache = tmp;
                cache_capacity = new_cap;
            }
            cache[cache_count++] = next_scan;
        }
        next_scan++;
    }
    return cache[n - 1];
}
