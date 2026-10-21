/*
 * A247752  Numbers in decimal representation, such that in Dutch their digits
 *          are in alphabetic order.
 * https://oeis.org/A247752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 1862, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247752: numbers whose decimal digits are in Dutch alphabetic order. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Dutch digit ranking (lexicographic order of Dutch names):
   8 < 1 < 3 < 9 < 0 < 2 < 4 < 5 < 6 < 7 */
static const unsigned char rank[10] = {4, 1, 5, 2, 6, 7, 8, 9, 0, 3};

/* Check whether x has its decimal digits in Dutch order.
   We traverse the digits from least‑significant to most‑significant;
   for Dutch order we require that the rank never increases in this
   direction (i.e. it never decreases in the natural order). */
static bool valid_digits(seq_t x)
{
    if (x == 0) return true;              /* single digit 0 is trivially OK */

    unsigned char last_rank = 255;         /* larger than any valid rank (0‑9) */
    while (x > 0) {
        unsigned char d = (unsigned char)(x % 10);   /* digit 0‑9 */
        unsigned char r = rank[d];
        if (r > last_rank) return false;            /* dip in rank */
        last_rank = r;
        x = x / 10;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */

    /* Static cache of previously found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_test = 0;    /* next integer to test */

    size_t needed = (size_t)n;
    if (needed > cache_len) {
        /* Grow cache capacity if necessary */
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1U : 16;
            while (new_cap < needed) new_cap <<= 1U;

#ifdef __cplusplus
            seq_t *tmp = new seq_t[new_cap];
#else
            seq_t *tmp = malloc(new_cap * sizeof(seq_t));
            if (!tmp) abort();
#endif
            /* Copy existing values */
            for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];

#ifdef __cplusplus
            delete[] cache;
#else
            free(cache);
#endif
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate new terms until we have enough */
        while (cache_len < needed) {
            seq_t val = (seq_t)next_test;
            if (valid_digits(val))
                cache[cache_len++] = val;
            ++next_test;
        }
    }

    return cache[n - 1];
}
