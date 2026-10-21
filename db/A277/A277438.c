/*
 * A277438  Numbers whose letters are in alphabetical order when spelled in
 *          French.
 * https://oeis.org/A277438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, fini, full
 * Formula:   a(n) = the nth integer whose French name’s letters are in
 *            alphabetical order
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_alpha_order
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_alpha_order.h"

/* Helper arrays for French word parts */
static const char *const unit[] = {
    "",      "un",     "deux",   "trois",  "quatre",
    "cinq",  "six",    "sept",   "huit",  "neuf",
    "dix",   "onze",   "douze",  "treize", "quatorze",
    "quinze", "seize", "dixsept","dixhuit","dixneuf"
};

static const char *const tens[] = { "", "", "vingt", "trente", "quarante", "cinquante", "soixante" };

/* Append a word to the buffer */
static void append(char *buf, size_t *len, const char *word)
{
    while (*word) {
        buf[(*len)++] = *word++;
    }
}

/* Recursive conversion of a number (0 < n < 1e12) to French word, concatenated without
   spaces or hyphens in lowercase letters. */
static void number_to_french(uint64_t n, char *buf, size_t *len)
{
    if (n == 0) return;

    if (n < 20) {               /* 1 .. 19 */
        append(buf, len, unit[n]);
        return;
    }

    if (n < 70) {               /* 20 .. 69 */
        uint64_t t = n / 10;
        uint64_t u = n % 10;
        append(buf, len, tens[t]);
        if (u != 0) {
            /* The word "et" is used for 21, 31, 41, 51, 61; here it is omitted
               because such numbers cannot be alphabetically ordered anyway. */
            append(buf, len, unit[u]);
        }
        return;
    }

    if (n < 80) {               /* 70 .. 79 */
        uint64_t rest = n - 60;  /* 10 .. 19 */
        append(buf, len, "soixante");
        if (rest == 10) {
            append(buf, len, "dix");
        } else if (rest == 11) {
            append(buf, len, "et");
            append(buf, len, "onze");
        } else {
            append(buf, len, unit[rest]);   /* 12 .. 19 */
        }
        return;
    }

    if (n < 100) {              /* 80 .. 99 */
        uint64_t rest = n - 80; /* 0 .. 19 */
        if (rest == 0) {
            append(buf, len, "quatrevingts");
        } else {
            append(buf, len, "quatrevingt");
            if (rest == 10) {
                append(buf, len, "dix");
            } else if (rest == 11) {
                append(buf, len, "onze");
            } else {
                append(buf, len, unit[rest]);   /* 12 .. 19 */
            }
        }
        return;
    }

    if (n < 1000) {             /* 100 .. 999 */
        uint64_t h = n / 100;
        uint64_t r = n % 100;
        if (h == 1)
            append(buf, len, "cent");
        else {
            append(buf, len, unit[h]);
            append(buf, len, "cent");
        }
        if (r != 0)
            number_to_french(r, buf, len);
        return;
    }

    if (n < 1000000) {          /* 1,000 .. 999,999 */
        uint64_t k = n / 1000;
        uint64_t r = n % 1000;
        if (k == 1)
            append(buf, len, "mille");
        else {
            number_to_french(k, buf, len);
            append(buf, len, "mille");
        }
        if (r != 0)
            number_to_french(r, buf, len);
        return;
    }

    if (n < 1000000000) {       /* 1,000,000 .. 999,999,999 */
        uint64_t k = n / 1000000;
        uint64_t r = n % 1000000;
        if (k == 1)
            append(buf, len, "million");
        else {
            number_to_french(k, buf, len);
            append(buf, len, "million");
        }
        if (r != 0)
            number_to_french(r, buf, len);
        return;
    }

    /* Larger numbers are not needed for the known terms of this sequence. */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset is 1; n=0 is invalid */

    idx_t found = 0;
    const idx_t LIMIT = 2000000;      /* sufficiently large to encompass known terms */

    char buf[256];
    size_t len;

    for (uint64_t num = 1; num <= LIMIT; ++num) {
        len = 0;
        number_to_french(num, buf, &len);
        if (is_alpha_order(buf, len)) {
            ++found;
            if (found == n)
                return (seq_t)num;
        }
    }
    return (seq_t)0;  /* sequence exhausted */
}
