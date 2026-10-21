/*
 * A282636  a(1) = 1. a(n) is the smallest natural number such that a(n) >
 *          a(n-1), the name of a(n) in (American) English ends with the
 *          letter a(n-1) starts with, and a(n) makes the sequence extendable.
 * https://oeis.org/A282636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   first_char, last_char
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A282636
   a(1)=1. For n>1, a(n) is the smallest integer >a(n‑1) whose
   English name ends with the letter with which a(n‑1) starts, and
   that name does not start with f or s. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/first_char.h"
#include "../utils/last_char.h"

/* English words for numbers below 1000. */
static const char *ONES[10] = {"zero","one","two","three","four","five",
                               "six","seven","eight","nine"};
static const char *TEENS[10] = {"ten","eleven","twelve","thirteen","fourteen",
                                "fifteen","sixteen","seventeen","eighteen",
                                "nineteen"};
static const char *TENS[10] = {"","","twenty","thirty","forty","fifty",
                               "sixty","seventy","eighty","ninety"};

#define WORDBUF_SZ 512 /* enough for any number that fits in seq_t */

/* Append a word to a buffer, separating by a space if needed. */
static void append_word(char *buf, size_t *pos, const char *w, size_t bufsize)
{
    size_t l = strlen(w);
    if (*pos > 0 && *pos + 1 < bufsize)
        buf[(*pos)++] = ' ';
    if (*pos + l < bufsize) {
        memcpy(buf + *pos, w, l);
        *pos += l;
    }
    if (*pos < bufsize)
        buf[*pos] = '\0';
}

/* Write the words for a number <1000 into the buffer. */
static void write_under_1000(seq_t n, char *buf, size_t *pos)
{
    if (n == (seq_t)0)
        return;

    seq_t hundreds = n / (seq_t)100;
    if (hundreds) {
        size_t h = (size_t)hundreds;
        append_word(buf, pos, ONES[h], WORDBUF_SZ);
        append_word(buf, pos, "hundred", WORDBUF_SZ);
        n %= (seq_t)100;
        if (n == (seq_t)0)
            return;
    }

    if (n >= (seq_t)20) {
        seq_t tens = n / (seq_t)10;
        size_t t = (size_t)tens;
        append_word(buf, pos, TENS[t], WORDBUF_SZ);
        n %= (seq_t)10;
        if (n == (seq_t)0)
            return;
    }

    if (n >= (seq_t)10) {
        size_t te = (size_t)(n - (seq_t)10);
        append_word(buf, pos, TEENS[te], WORDBUF_SZ);
    } else if (n > (seq_t)0) {
        size_t o = (size_t)n;
        append_word(buf, pos, ONES[o], WORDBUF_SZ);
    }
}

/* Convert a seq_t number to its American English name. */
static void num_to_words(seq_t n, char *buf, size_t bufsize)
{
    size_t pos = 0;
    if (n == (seq_t)0) {
        append_word(buf, &pos, "zero", bufsize);
        return;
    }

    /* Use a wider type for the heavy arithmetic to avoid overflow
       when seq_t is very small (e.g. uint8_t). */
    uint64_t x = (uint64_t)n;

    uint64_t billions = x / 1000000000ULL;
    uint64_t millions  = (x / 1000000ULL) % 1000ULL;
    uint64_t thousands = (x / 1000ULL) % 1000ULL;
    uint64_t remainder = x % 1000ULL;

    if (billions) {
        write_under_1000((seq_t)billions, buf, &pos);
        append_word(buf, &pos, "billion", bufsize);
    }
    if (millions) {
        write_under_1000((seq_t)millions, buf, &pos);
        append_word(buf, &pos, "million", bufsize);
    }
    if (thousands) {
        write_under_1000((seq_t)thousands, buf, &pos);
        append_word(buf, &pos, "thousand", bufsize);
    }
    if (remainder) {
        write_under_1000((seq_t)remainder, buf, &pos);
    }
}

/* Memoised computation of a(n). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined but harmless */

    /* Static cache: values and the starting letter of each term. */
    static seq_t *cache = NULL;
    static char *start_letters = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;

    /* Ensure capacity for up to n terms. */
    if (cache_size + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n)
            new_cap = n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        start_letters = (char *)realloc(start_letters,
                                        new_cap * sizeof(char));
        cache_cap = new_cap;
    }

    /* Compute terms up to n. */
    while (cache_size < (size_t)n) {
        if (cache_size == 0) {           /* first term: a(1) = 1 */
            cache[0] = (seq_t)1;
            start_letters[0] = first_char("one");
            cache_size = 1;
            continue;
        }

        seq_t prev = cache[cache_size - 1];
        char   prev_start = start_letters[cache_size - 1];
        seq_t cand = prev + (seq_t)1;
        char buf[WORDBUF_SZ];

        for (;;) {
            num_to_words(cand, buf, sizeof(buf));
            char curr_start = first_char(buf);
            if (curr_start == 'f' || curr_start == 's')
                { cand++; continue; }
            char curr_end = last_char(buf);
            if (curr_end == prev_start)
                break;
            cand++;
        }

        cache[cache_size] = cand;
        start_letters[cache_size] = first_char(buf); /* start letter for next term */
        cache_size++;
    }

    return cache[n - 1];
}
