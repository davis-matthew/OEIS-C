/*
 * A351025  a(n) is the rank of the US English name of n when all its letters
 *          are written in all possible orders and arranged in alphabetic
 *          order.
 * https://oeis.org/A351025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = rank of the US English word for n (without spaces and
 *            hyphens) among all permutations of its letters in alphabetical
 *            order
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   add_token
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include "../utils/add_token.h"

/* String tables for number words (US English, short scale, no "and") */
static const char *UNITS[10] = {
    "ZERO","ONE","TWO","THREE","FOUR","FIVE","SIX","SEVEN","EIGHT","NINE"
};

static const char *TEENS[10] = {
    "TEN","ELEVEN","TWELVE","THIRTEEN","FOURTEEN","FIFTEEN",
    "SIXTEEN","SEVENTEEN","EIGHTEEN","NINETEEN"
};

static const char *TENS[10] = {
    "", "", "TWENTY","THIRTY","FORTY","FIFTY","SIXTY",
    "SEVENTY","EIGHTY","NINETY"
};

static const char *SCALES[6] = {
    "", "THOUSAND","MILLION","BILLION","TRILLION","QUADRILLION"
};

/* Append the word for a number less than 1000 (excluding spaces). */
static void wordsBelow1000(uint64_t n, char *buf, size_t *len)
{
    if (n >= 100) {
        uint64_t h = n / 100;
        add_token(UNITS[h], buf, len);
        add_token("HUNDRED", buf, len);
        n %= 100;
    }
    if (n >= 20) {
        uint64_t t = n / 10;
        add_token(TENS[t], buf, len);
        n %= 10;
    }
    if (n >= 10) {
        add_token(TEENS[n - 10], buf, len);
    } else if (n > 0) {
        add_token(UNITS[n], buf, len);
    }
}

/* Convert n to its US English word (letters only, upper‑case) into buf. */
static void number_to_letters(idx_t n, char *buf, size_t *len)
{
    *len = 0;
    if (n == 0) {
        add_token(UNITS[0], buf, len);   /* "ZERO" */
        return;
    }

    uint64_t tmp = (uint64_t)n;
    uint64_t group[10];
    int ng = 0;
    while (tmp) {
        group[ng++] = tmp % 1000;
        tmp /= 1000;
    }

    for (int i = ng - 1; i >= 0; --i) {
        if (group[i] == 0) continue;
        wordsBelow1000(group[i], buf, len);
        if (SCALES[i][0]) {
            add_token(SCALES[i], buf, len);
        }
    }
}

/* Pre‑computed factorials up to 512 (sufficient for all inputs). */
static seq_t fact_cache[512];

seq_t a(idx_t n)
{
    char letters[512];
    size_t len;
    number_to_letters(n, letters, &len);

    /* Letter frequency table */
    unsigned char count[26] = {0};
    for (size_t i = 0; i < len; ++i)
        ++count[letters[i] - 'A'];

    /* Factorials up to len */
    fact_cache[0] = (seq_t)1;
    for (size_t i = 1; i <= len; ++i)
        fact_cache[i] = fact_cache[i-1] * (seq_t)i;

    /* Lexicographic rank among all permutations of the letters */
    seq_t rank = 1;
    size_t remaining = len;
    for (size_t pos = 0; pos < len; ++pos) {
        int cur = letters[pos] - 'A';
        for (int l = 0; l < cur; ++l) {
            if (!count[l]) continue;
            --count[l];
            seq_t perms = fact_cache[remaining - 1];
            for (int k = 0; k < 26; ++k) {
                if (count[k] > 1)
                    perms /= fact_cache[count[k]];
            }
            rank += perms;
            ++count[l];
        }
        --count[cur];
        --remaining;
    }
    return rank;
}
