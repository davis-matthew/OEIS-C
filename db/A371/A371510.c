/*
 * A371510  Number of swaps needed to bubble-sort the US English name of n.
 * https://oeis.org/A371510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, word
 * Formula:   a(n) = number of inversions in the alphabetic sequence of
 *            letters * in the US English name of n, ignoring hyphens and
 *            spaces. * In other words, count the adjacent swaps needed to
 *            bubble‑sort * the letters of the name
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   _count_inversions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/_count_inversions.h"

/* Append a null‑terminated word to buf[0..*len-1]. */
static void append_word(char *buf, size_t *len, const char *w)
{
    while (*w) {
        buf[(*len)++] = *w++;
    }
}

/* Words for 1–19 (index 0 unused). */
static const char *const _w_1_19[20] = {
    "", "one", "two", "three", "four", "five", "six", "seven",
    "eight", "nine", "ten", "eleven", "twelve", "thirteen",
    "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
};
/* Words for tens 20,30,…,90 (index 0,1 unused). */
static const char *const _tens[10] = {
    "", "", "twenty", "thirty", "forty", "fifty",
    "sixty", "seventy", "eighty", "ninety"
};
/* Scale names (group 1 = thousand). */
static const char *const _scale[16] = {
    "", "thousand", "million", "billion", "trillion", "quadrillion",
    "quintillion", "sextillion", "septillion", "octillion", "nonillion",
    "decillion", "undecillion", "duodecillion", "tredecillion", ""
};

/* Build the word segment for a 3‑digit group 0 < n ≤ 999,
 * appending into buf. Assumes no hyphens or spaces are added. */
static void _build_group(idx_t n, char *buf, size_t *len)
{
    if (n >= 100) {
        idx_t h = n / 100;
        append_word(buf, len, _w_1_19[h]);   /* hundreds digit word */
        append_word(buf, len, "hundred");
        n %= 100;
        if (n == 0) return;
    }
    if (n >= 20) {
        idx_t t = n / 10;
        append_word(buf, len, _tens[t]);      /* tens word */
        n %= 10;
        if (n == 0) return;
    }
    if (n > 0) {
        /* 1–19 */
        append_word(buf, len, _w_1_19[n]);
    }
}

/* Build the full US English name of n (0‑based). */
static void _build_name(idx_t n, char *buf, size_t *len)
{
    if (n == 0) {
        append_word(buf, len, "zero");
        return;
    }

    /* Split n into 3‑digit groups (LSB first). */
    idx_t groups[32];
    size_t gcnt = 0;
    while (n > 0) {
        groups[gcnt++] = n % 1000;
        n /= 1000;
    }

    /* Emit groups from most significant to least. */
    for (size_t i = gcnt; i > 0; --i) {
        idx_t g = groups[i - 1];
        if (g == 0) continue;
        _build_group(g, buf, len);
        if (i - 1 > 0) {            /* add scale name if not the units group */
            const char *s = _scale[i - 1];
            append_word(buf, len, s);
        }
    }
}

/* The public sequence function. */
seq_t a(idx_t n)
{
    /* Sufficiently large buffer for all words of n up to very large values. */
    char buf[4096];
    size_t len = 0;

    _build_name(n, buf, &len);
    return _count_inversions(buf, len);
}
