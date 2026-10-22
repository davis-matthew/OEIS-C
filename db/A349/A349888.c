/*
 * A349888  Numbers which, when written in English, contain exactly two
 *          instances of the letter E.
 * https://oeis.org/A349888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 6469, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) by scanning natural numbers and counting the letter 'e'
    in their English (lower‑case) representation.
    a(1) = 3, a(2) = 7, a(3) = 12, ...
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Small number word tables                                         */
/* ------------------------------------------------------------------ */
static const char *ONES[10] = {
    "zero", "one", "two", "three", "four",
    "five", "six", "seven", "eight", "nine"
};

static const char *TEENS[10] = {
    "ten", "eleven", "twelve", "thirteen", "fourteen",
    "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
};

static const char *TENS[10] = {
    "", "", "twenty", "thirty", "forty",
    "fifty", "sixty", "seventy", "eighty", "ninety"
};

static const char *SCALES[] = {
    "", " thousand", " million", " billion", " trillion",
    " quadrillion", " quintillion", " sextillion", " septillion",
    " octillion", " nonillion", " decillion"
};

/* ------------------------------------------------------------------ */
/* Count the letter 'e' in the English spelling of n (in lower case) */
/* ------------------------------------------------------------------ */
static int count_es(idx_t n) {
    char buf[512];
    size_t pos = 0;

    if (n == 0) {
        pos += snprintf(buf + pos, sizeof(buf) - pos, "zero");
    } else {
        /* split n into 3‑digit groups */
        idx_t groups[20];
        int gcnt = 0;
        idx_t tmp = n;
        while (tmp > 0) {
            groups[gcnt++] = tmp % 1000;
            tmp /= 1000;
        }

        for (int i = gcnt - 1; i >= 0; --i) {
            idx_t chunk = groups[i];
            if (chunk == 0)
                continue;

            if (pos > 0)
                pos += snprintf(buf + pos, sizeof(buf) - pos, " ");

            /* hundreds */
            if (chunk >= 100) {
                int h = chunk / 100;
                pos += snprintf(buf + pos, sizeof(buf) - pos, "%s hundred", ONES[h]);
                chunk %= 100;
                if (chunk)
                    pos += snprintf(buf + pos, sizeof(buf) - pos, " ");
            }

            /* tens and teens */
            if (chunk >= 20) {
                int t = chunk / 10;
                pos += snprintf(buf + pos, sizeof(buf) - pos, "%s", TENS[t]);
                chunk %= 10;
                if (chunk)
                    pos += snprintf(buf + pos, sizeof(buf) - pos, " ");
            }

            if (chunk >= 10) {
                pos += snprintf(buf + pos, sizeof(buf) - pos, "%s", TEENS[chunk - 10]);
                chunk = 0;
            }

            /* units */
            if (chunk > 0 && chunk < 10) {
                pos += snprintf(buf + pos, sizeof(buf) - pos, "%s", ONES[chunk]);
            }

            /* scale */
            if (i > 0)
                pos += snprintf(buf + pos, sizeof(buf) - pos, "%s", SCALES[i]);
        }
    }

    /* count 'e' or 'E' */
    int count = 0;
    for (size_t i = 0; i < pos; ++i) {
        if (buf[i] == 'e' || buf[i] == 'E')
            ++count;
    }
    return count;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t last_checked = 0;

    if (n <= 0)
        return 0; /* undefined for n < 1 */

    /* Grow the sequence until it reaches n terms */
    for (idx_t num = last_checked + 1; seq_len < n; ++num) {
        if (count_es(num) == 2) {
            if (seq_len == seq_cap) {
                seq_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)num;
        }
        last_checked = num;
    }

    return seq[n - 1];
}
