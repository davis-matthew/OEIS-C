/*
 * A362122  a(n) is the smallest positive number whose British English name
 *          has the letter "e" in the n-th position.
 * https://oeis.org/A362122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the smallest positive number whose British English name has the
   letter “e” in the n‑th position.  The function scans natural numbers
   sequentially and records the first number that gives an “e” at each
   character position.  Whenever a larger n is requested the search
   continues from the last examined number.  The implementation is
   agnostic to whether seq_t is an integer type or an arbitrary‑precision
   class.  It uses the helper function seq_realloc from
   ../utils/seq_realloc.h to manage a dynamic array of seq_t values.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

static const char *const units[10] = {
    "", "one", "two", "three", "four",
    "five", "six", "seven", "eight", "nine" };

static const char *const teens[10] = {
    "ten", "eleven", "twelve", "thirteen", "fourteen",
    "fifteen", "sixteen", "seventeen", "eighteen", "nineteen" };

static const char *const tens[10] = {
    "", "", "twenty", "thirty", "forty",
    "fifty", "sixty", "seventy", "eighty", "ninety" };

static const char *const scale_names[20] = {
    "", "thousand", "million", "billion", "trillion",
    "quadrillion", "quintillion", "sextillion", "septillion",
    "octillion", "nonillion", "decillion", "undecillion",
    "duodecillion", "tredecillion", "quattuordecillion",
    "quindecillion", "sexdecillion", "septendecillion",
    "octodecillion" };

/* Append a word (potentially multi‑char) into buf, updating len.  */
static inline void append_word(const char *w, char *buf, size_t *len)
{
    while (*w) buf[(*len)++] = *w++;
}

/* Append a three‑digit group (0–999) into buf. */
static inline void append_group(uint32_t n, char *buf, size_t *len)
{
    uint32_t hundreds = n / 100;
    uint32_t rest     = n % 100;

    if (hundreds) {
        append_word(units[hundreds], buf, len);
        append_word("hundred", buf, len);
        if (rest) append_word("and", buf, len);
    }

    if (rest < 10) {
        if (rest) append_word(units[rest], buf, len);
    } else if (rest < 20) {
        append_word(teens[rest - 10], buf, len);
    } else {
        uint32_t t = rest / 10;
        uint32_t u = rest % 10;
        append_word(tens[t], buf, len);
        if (u) append_word(units[u], buf, len);
    }
}

/* Build the concatenated word form of num (no spaces, no hyphens). */
static void name_to_words(seq_t num, char *buf, size_t *len)
{
    /* Decompose num into groups of three digits (little‑endian). */
    uint32_t groups[20];
    size_t count = 0;
    seq_t tmp = num;
    while (tmp) {
        groups[count++] = (uint32_t)(tmp % (seq_t)1000);
        tmp /= (seq_t)1000;
    }
    if (count == 0) {                 /* never happens for positive num */
        append_word("zero", buf, len);
        return;
    }

    /* Process from most significant group to least. */
    for (int idx = (int)count - 1; idx >= 0; --idx) {
        uint32_t grp = groups[idx];
        if (!grp) continue;            /* skip zero groups                */

        /* Leading "and" before the lowest non‑zero group if needed. */
        if (idx == 0 && count > 1 && grp < 100)
            append_word("and", buf, len);

        append_group(grp, buf, len);

        if (idx > 0) {
            const char *scale = scale_names[idx];
            if (*scale) append_word(scale, buf, len);
        }
    }
}

/* -------------------- memoisation and state -------------------- */

static seq_t *memo_vals = NULL;   /* indices 1..   , memo_vals[0] unused  */
static size_t memo_cap  = 0;      /* current allocated size                */
static seq_t  next_scan = 1;      /* next number to examine                */

/* Ensure capacity for at least 'need' indices (1‑based). */
static inline void ensure_capacity(size_t need)
{
    if (need < memo_cap) return;
    size_t old_cap = memo_cap;
    seq_t *new_mem = seq_realloc(memo_vals, old_cap, need);
    if (!new_mem) abort();
    memo_vals = new_mem;
    memo_cap = need;
    /* initialise new slots to 0 (zero is default for seq_t) */
    for (size_t i = old_cap; i < memo_cap; ++i)
        memo_vals[i] = 0;
}

/* --------------------- public function --------------------- */

seq_t a(idx_t n)
{
    if ((idx_t)0 == n) return 0;               /* undefined, safe guard      */

    /* Expand memo array to hold up to index n. */
    ensure_capacity((size_t)n + 1);

    /* If already known, return immediately. */
    if (memo_vals[(size_t)n] != (seq_t)0) return memo_vals[(size_t)n];

    /* Static buffer big enough for any number we will encounter. */
    static char work_buf[8192];
    size_t work_len;

    /* Scan numbers until the desired position becomes known. */
    for (;;) {
        seq_t cur = next_scan++;

        /* Build word representation. */
        work_len = 0;
        name_to_words(cur, work_buf, &work_len);

        /* Record positions where 'e' occurs. */
        for (size_t i = 0; i < work_len; ++i) {
            if (work_buf[i] == 'e') {
                size_t pos = i + 1;            /* 1‑based                */
                ensure_capacity(pos + 1);
                seq_t existing = memo_vals[pos];
                if (existing == (seq_t)0 || cur < existing)
                    memo_vals[pos] = cur;
            }
        }

        if (memo_vals[(size_t)n] != (seq_t)0) return memo_vals[(size_t)n];
    }
}
