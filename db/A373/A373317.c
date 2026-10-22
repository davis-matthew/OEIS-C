/*
 * A373317  Numbers which are their own imbalance when written in English (see
 *          the definition of the imbalance in the Comments section).
 * https://oeis.org/A373317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, word, fini, full
 * Formula:   a(n) = nth term of OEIS A373317 (numbers that are their own
 *            imbalance when written in English). Computed by exhaustive
 *            search
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/seq_realloc.h"

/* Mapping of single digits and tens to words in capital letters. */
static const char *const unit_words[20] = {
    "", "ONE","TWO","THREE","FOUR","FIVE","SIX","SEVEN","EIGHT","NINE",
    "TEN","ELEVEN","TWELVE","THIRTEEN","FOURTEEN","FIFTEEN",
    "SIXTEEN","SEVENTEEN","EIGHTEEN","NINETEEN"
};
static const char *const ten_words[10] = {
    "", "", "TWENTY","THIRTY","FORTY","FIFTY",
    "SIXTY","SEVENTY","EIGHTY","NINETY"
};
static const char *const thousand_words[5] = {
    "", "THOUSAND","MILLION","BILLION","TRILLION"
};

/* Append a null‑terminated word to buffer at position *pos. */
static void append_word(char *buf, size_t *pos, const char *word)
{
    while (*word) buf[(*pos)++] = *word++;
}

/* Convert a 3‑digit group into words.  idx denotes the thousand group
   (0 = units, 1 = thousand, 2 = million, ...). */
static void group_to_words(uint16_t g, int idx, char *buf, size_t *pos)
{
    if (g == 0) return;
    uint16_t h = g / 100;           /* hundreds */
    uint16_t r = g % 100;           /* remainder 0‑99 */
    if (h) {
        append_word(buf, pos, unit_words[h]);
        append_word(buf, pos, "HUNDRED");
    }
    if (r >= 10 && r < 20) {
        append_word(buf, pos, unit_words[r]);          /* 10‑19 */
    } else {
        if (r >= 20) {
            uint16_t t = r / 10;
            append_word(buf, pos, ten_words[t]);       /* tens */
            r %= 10;
        }
        if (r) {
            append_word(buf, pos, unit_words[r]);     /* units */
        }
    }
    if (idx) {
        append_word(buf, pos, thousand_words[idx]);   /* thousand/million … */
    }
}

/* Convert an unsigned integer n (0 < n < 1e15) to its English name in
   capital letters, without spaces or hyphens.  The result is written into
   the supplied buffer (size >= 512).  */
static void number_to_english(uint64_t n, char *buf)
{
    uint16_t groups[6];
    int count = 0;
    while (n) {
        groups[count++] = n % 1000;
        n /= 1000;
    }
    size_t pos = 0;
    for (int i = count - 1; i >= 0; --i) {
        group_to_words(groups[i], i, buf, &pos);
    }
    buf[pos] = '\0';
}

/* Return true if num is equal to an imbalance of its English name.  */
static bool is_own_imbalance(uint64_t num)
{
    char word[512];
    number_to_english(num, word);

    /* Compute total valuation (sum of letter values A=1 .. Z=26). */
    uint64_t total = 0;
    for (const char *p = word; *p; ++p) {
        total += (uint64_t)(*p - 'A' + 1);
    }

    /* Scan all cuts: before each letter, including ends. */
    uint64_t left = 0;
    for (const char *p = word; ; ++p) {
        uint64_t right = total - left;
        uint64_t diff = right > left ? right - left : left - right;
        if (diff == num) return true;
        if (*p == '\0') break;
        left += (uint64_t)(*p - 'A' + 1);
    }
    return false;
}

/* Cache of discovered terms. */
static seq_t *terms = NULL;
static size_t terms_size = 0;
static size_t terms_cap  = 0;
static uint64_t next_candidate = 1ULL;

/* Grow the terms array using seq_realloc (works for arbitrary‐precision seq_t). */
static void grow_terms(void)
{
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    terms = seq_realloc(terms, terms_cap, new_cap);
    terms_cap = new_cap;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms. */
    while (terms_size < (size_t)n) {
        if (is_own_imbalance(next_candidate)) {
            if (terms_size == terms_cap) grow_terms();
            terms[terms_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return terms[n - 1];
}
