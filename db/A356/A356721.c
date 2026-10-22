/*
 * A356721  Numbers written using exactly two distinct Roman numerals.
 * https://oeis.org/A356721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers written using exactly two distinct Roman numerals. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Predefined Roman numeral values and symbols (standard subtractive notation). */
static const int roman_vals[13] = {1000,900,500,400,100,90,50,40,10,9,5,4,1};
static const char *roman_syms[13] = {"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};

/* Convert index n (idx_t) to the nth term of the sequence. */
seq_t a(idx_t n) {
    if ((idx_t)0 == n) return 0;          /* defined only for n≥1 */

    uint64_t target = (uint64_t)n;

    /* Static cache to store already computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    /* Static variable keeping track of the next integer to test. */
    static uint64_t next_candidate = 1;

    /* Mapping from Roman characters to bit positions (I,V,X,L,C,D,M). */
    static int char_bit[256];
    static bool init = false;
    if (!init) {
        init = true;
        for (int i = 0; i < 256; ++i) char_bit[i] = 0;
        char_bit[(unsigned char)'I'] = 1 << 0;
        char_bit[(unsigned char)'V'] = 1 << 1;
        char_bit[(unsigned char)'X'] = 1 << 2;
        char_bit[(unsigned char)'L'] = 1 << 3;
        char_bit[(unsigned char)'C'] = 1 << 4;
        char_bit[(unsigned char)'D'] = 1 << 5;
        char_bit[(unsigned char)'M'] = 1 << 6;
    }

    if (target <= cache_size) return cache[target - 1];

    /* Ensure cache has initial capacity. */
    if (cache_cap == 0) {
        cache_cap = 32;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure */
    }

    /* Generate terms until we have at least 'target' terms. */
    while (cache_size < target) {
        uint64_t num = next_candidate;
        uint64_t val = num;
        uint8_t mask = 0;

        /* Convert number to Roman numerals, building a bit mask of used symbols. */
        for (int i = 0; i < 13; ++i) {
            while (val >= roman_vals[i]) {
                const char *s = roman_syms[i];
                for (const char *p = s; *p; ++p) {
                    mask |= (uint8_t)char_bit[(unsigned char)*p];
                }
                val -= roman_vals[i];
            }
        }

        /* Count the number of distinct symbols used. */
        int bits = 0;
        uint8_t t = mask;
        while (t) {
            bits++;
            t &= t - 1;
        }

        if (bits == 2) {
            /* Store term in cache, resizing if necessary. */
            if (cache_size >= cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure */
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)num;
        }
        next_candidate++;
    }

    return cache[target - 1];
}
