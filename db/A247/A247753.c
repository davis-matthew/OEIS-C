/*
 * A247753  Numbers in decimal representation, such that in Finnish their
 *          digits are in alphabetic order.
 * https://oeis.org/A247753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word
 * Formula:   by iterating through integers and selecting those whose decimal
 *            digits, in the order most‑significant to least‑significant, have
 *            non‑decreasing ranks according to Finnish alphabet order. The
 *            rank order of digits is: 8(kahdeksan), 2(kaksi), 3(kolme),
 *            6(kuusi), 4(neljä), 0(nolla), 7(seits­emän), 5(...
 * Range:     u8 n <= 92, u16 n <= 2006, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Rank mapping for decimal digits according to Finnish alphabetical order */
static const int rank_map[10] = {5, 9, 1, 2, 4, 7, 3, 6, 0, 8};

/* Check whether all digits of x are in non‑decreasing order of rank
   when read from most significant to least significant. */
static bool valid_finnish_digits(seq_t x)
{
    if (x == (seq_t)0)            /* 0 is always valid */
        return true;

    int prev_rank = 10;           /* larger than any rank */
    seq_t t = x;
    while (t != (seq_t)0) {
        int digit = (int)(t % (seq_t)10);   /* extract least significant digit */
        int r = rank_map[digit];
        if (r > prev_rank)                /* earlier digit has larger rank → invalid */
            return false;
        prev_rank = r;
        t /= (seq_t)10;
    }
    return true;
}

/* Static cache for computed sequence terms */
static seq_t *cache = NULL;       /* dynamic array of terms */
static size_t cache_cap = 0;      /* current allocated capacity */
static idx_t cache_len = 0;       /* number of terms stored */
static seq_t next_val = (seq_t)0; /* next integer to test */

/* Public function computing a(n) for the Finnish‑ordered digit sequence */
seq_t a(idx_t n)
{
    /* Bounds: the sequence is defined for n >= 1. */
    if (n == 0) return (seq_t)0; /* Undefined, return 0 as safety */

    /* If the desired term is already computed, return it immediately. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Extend the cache until it contains at least n terms. */
    for (; cache_len < n; ) {
        /* Test current candidate */
        if (valid_finnish_digits(next_val)) {
            /* Ensure sufficient capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure: terminate with diagnostic */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the valid term */
            cache[cache_len++] = next_val;
        }
        /* Advance to the next integer candidate */
        next_val += (seq_t)1;
    }

    return cache[n - 1];
}
