/*
 * A389982  Triangle in which row n contains values from 1 to n in
 *          alphabetical order (in German).
 * https://oeis.org/A389982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, word
 * Formula:   a(n) is the n-th element of the triangle formed by sorting each
 *            row r (containing the numbers 1..r) alphabetically by the German
 *            word representations of the numbers
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Helper that writes the German word (ASCII approximation) for a number.
   The function works for numbers up to 999 (enough for all n up to 10000). */
static void write_german_word(idx_t n, char *buf)
{
    /* Words for 1..19 */
    static const char *short_words[20] = {
        "",      /* 0 unused */
        "eins",  /* 1 */
        "zwei",  /* 2 */
        "drei",  /* 3 */
        "vier",  /* 4 */
        "fuenf", /* 5 */
        "sechs", /* 6 */
        "sieben",/* 7 */
        "acht",  /* 8 */
        "neun",  /* 9 */
        "zehn",  /*10 */
        "elf",   /*11 */
        "zwolf", /*12 */
        "dreizehn",/*13*/
        "vierzehn",/*14*/
        "fuenfzehn",/*15*/
        "sechzehn", /*16*/
        "siebzehn", /*17*/
        "achtzehn", /*18*/
        "neunzehn"  /*19*/
    };

    static const char *tens_words[10] = {
        "", "",                   /* 0,1 unused */
        "zwanzig",                /*20*/
        "dreissig",               /*30*/
        "vierzig",                /*40*/
        "fuenfzig",               /*50*/
        "sechzig",                /*60*/
        "siebzig",                /*70*/
        "achtzig",                /*80*/
        "neunzig"                 /*90*/
    };

    static const char *unit_words[10] = {
        "",       /* 0 unused */
        "eins",
        "zwei",
        "drei",
        "vier",
        "fuenf",
        "sechs",
        "sieben",
        "acht",
        "neun"
    };

    if (n <= 19) {
        strcpy(buf, short_words[(int)n]);
        return;
    }

    if (n < 100) {
        int tens = (int)(n / 10);
        int rem  = (int)(n % 10);
        if (rem == 0) {
            strcpy(buf, tens_words[tens]);
            return;
        }
        const char *unit = (rem == 1) ? "ein" : unit_words[rem];
        strcpy(buf, unit);
        strcat(buf, "und");
        strcat(buf, tens_words[tens]);
        return;
    }

    /* n >= 100 && n < 1000 */
    int hundred = (int)(n / 100);
    int rem     = (int)(n % 100);
    const char *prefix = (hundred == 1) ? "ein" : unit_words[hundred];
    strcpy(buf, prefix);
    strcat(buf, "hundert");
    if (rem > 0) {
        write_german_word((idx_t)rem, buf + strlen(buf));
    }
}

/* ----------------------------------------------------------------------- */

/* Item used for sorting one row */
struct Item {
    uint64_t val;
    char     word[128];
};

/* Comparator for qsort */
static int cmp_item(const void *p, const void *q)
{
    const struct Item *a = (const struct Item *)p;
    const struct Item *b = (const struct Item *)q;
    return strcmp(a->word, b->word);
}

/* ----------------------------------------------------------------------- */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) {
        ++r;
    }

    /* Position within the row (1‑based) */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t pos = n - prev_tri;          /* 1‑based position in row r */

    /* Allocate array for the row */
    struct Item *items = (struct Item *)malloc((size_t)r * sizeof(struct Item));
    if (!items) {
        return (seq_t)0; /* allocation failure – unlikely for given limits */
    }

    /* Build the items */
    for (idx_t i = 1; i <= r; ++i) {
        items[i - 1].val = (uint64_t)i;
        write_german_word(i, items[i - 1].word);
    }

    /* Sort by German word */
    qsort(items, (size_t)r, sizeof(struct Item), cmp_item);

    /* Result */
    uint64_t result = items[pos - 1].val;
    free(items);
    return (seq_t)result;
}
