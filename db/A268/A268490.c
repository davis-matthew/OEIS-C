/*
 * A268490  Spelling out the characters (digits and commas) of the sequence
 *          and replacing letters A..Z with numbers 1..26 gives back the
 *          sequence.
 * https://oeis.org/A268490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268490:  a(1) = 20.
   For n ≥ 1, let v = a(n).  Write the decimal digits of v.
   For each digit d, append the word for d
     (ZERO, ONE, TWO, …, NINE) as characters.
   After all digits, append the word COMMA.
   Each character is mapped to its alphabetic value (A→1 … Z→26)
   and appended to the sequence.
   The first expansion skips the first letter of the word for the
   leading digit of a(1) because that letter is already the first term. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Digit words */
static const char *digit_words[10] = {
    "ZERO", "ONE", "TWO", "THREE", "FOUR",
    "FIVE", "SIX", "SEVEN", "EIGHT", "NINE"
};
static const char *comma_word = "COMMA";

/* Push a value into the static cache, growing it if necessary. */
static void push_cache(seq_t **cache, size_t *cap, size_t *len, seq_t val)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap ? *cap * 2 : 16);
        seq_t *new_arr = (seq_t*)realloc(*cache, new_cap * sizeof(seq_t));
        if (!new_arr) abort();  /* memory exhausted */
        *cache = new_arr;
        *cap = new_cap;
    }
    (*cache)[(*len)++] = val;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static size_t next_to_process = 0;  /* index of next term to expand */

    /* Initialise with the first term a(1)=20 */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)20;  /* 'T' */
        cache_len = 1;
        next_to_process = 0;
    }

    /* Expand terms until we have at least n terms */
    while (cache_len < n) {
        /* Process term at index next_to_process */
        seq_t val = cache[next_to_process];

        /* Convert val to decimal digits (most significant first) */
        uint64_t uv = (uint64_t)val;  /* safe: val is 1..26 */
        int digits[4];
        size_t dcnt = 0;
        if (uv == 0) {
            digits[dcnt++] = 0;
        } else {
            while (uv > 0) {
                digits[dcnt++] = uv % 10;
                uv /= 10;
            }
            /* reverse to most significant first */
            for (size_t i = 0; i < dcnt / 2; ++i) {
                int tmp = digits[i];
                digits[i] = digits[dcnt - 1 - i];
                digits[dcnt - 1 - i] = tmp;
            }
        }

        /* Skip the first letter only for the very first expansion */
        int skip_first = (next_to_process == 0) ? 1 : 0;

        /* Expand each digit word */
        for (size_t di = 0; di < dcnt; ++di) {
            const char *w = digit_words[digits[di]];
            for (const char *p = w; *p; ++p) {
                if (skip_first) {
                    skip_first = 0;
                    continue;
                }
                seq_t mapped = (seq_t)((unsigned char)(*p) - 'A' + 1);
                push_cache(&cache, &cache_cap, &cache_len, mapped);
            }
        }

        /* Append the word COMMA */
        for (const char *p = comma_word; *p; ++p) {
            seq_t mapped = (seq_t)((unsigned char)(*p) - 'A' + 1);
            push_cache(&cache, &cache_cap, &cache_len, mapped);
        }

        ++next_to_process;
    }

    return cache[n - 1];
}
