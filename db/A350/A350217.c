/*
 * A350217  a(1) = 1; a(n) > a(n-1) is the smallest number whose name in
 *          English does not contain the first letter of the name of a(n-1).
 * https://oeis.org/A350217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A350217 by searching for the smallest number greater than the
   previous term whose English name does not contain the first letter of the
   previous term's name. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

static const char *ONES[] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};
static const char *TENS[] = {
    "", "", "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
};
static const char *SCALES[] = {
    "", "thousand","million","billion","trillion","quadrillion",
    "quintillion","sextillion","septillion","octillion","nonillion",
    "decillion"
};

/* write the English words for a number <1000 into `out` (NULL-terminated) */
static void process_group(uint16_t g, char *out, size_t outsize)
{
    size_t pos = 0;
    if (g >= 100) {
        pos += snprintf(out + pos, outsize - pos, "%s hundred", ONES[g / 100]);
        if (g % 100) pos += snprintf(out + pos, outsize - pos, " ");
    }
    uint16_t rem = g % 100;
    if (rem >= 20) {
        pos += snprintf(out + pos, outsize - pos, "%s", TENS[rem / 10]);
        if (rem % 10)
            pos += snprintf(out + pos, outsize - pos, "-%s", ONES[rem % 10]);
    } else if (rem > 0) {
        pos += snprintf(out + pos, outsize - pos, "%s", ONES[rem]);
    }
    (void)pos;
}

/* build the full English name for `n` into `buf` (assumed size >= 512) */
static void name_of(uint64_t n, char *buf)
{
    char *p = buf;
    bool first = true;
    size_t idx = 0;
    while (n > 0) {
        uint16_t group = n % 1000;
        n /= 1000;
        if (group > 0) {
            if (!first)
                *p++ = ' ';
            first = false;
            char tmp[64];
            process_group(group, tmp, sizeof(tmp));
            size_t len = strlen(tmp);
            memcpy(p, tmp, len);
            p += len;
            if (SCALES[idx][0]) {
                *p++ = ' ';
                size_t ln = strlen(SCALES[idx]);
                memcpy(p, SCALES[idx], ln);
                p += ln;
            }
        }
        idx++;
    }
    if (first) {              /* n was 0 */
        strcpy(p, "zero");
        p += 4;
    }
    *p = '\0';
}

/* return the first alphabetic letter of the English name of `x` */
static char first_letter_of_name(seq_t x)
{
    char buf[512];
    name_of((uint64_t)x, buf);
    for (char *s = buf; *s; ++s) {
        if (isalpha((unsigned char)*s))
            return (char)tolower((unsigned char)*s);
    }
    return '\0';
}

/* true if the English name of `x` contains the letter `l` (case‑insensitive) */
static bool name_contains_letter(seq_t x, char l)
{
    char buf[512];
    name_of((uint64_t)x, buf);
    for (char *s = buf; *s; ++s) {
        if (isalpha((unsigned char)*s) && tolower((unsigned char)*s) == l)
            return true;
    }
    return false;
}

/* cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_sz = 0;   /* number of terms currently cached (≥1) or 0 */
static size_t cache_cap = 0;  /* allocation capacity */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n<1 */

    /* grow cache if needed */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = new_cap;
    }

    /* initialise first term if cache empty */
    if (cache_sz == 0) {
        cache[1] = 1;
        cache_sz = 1;
    }

    /* generate new terms up to n */
    for (idx_t i = cache_sz + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        char forbid = first_letter_of_name(prev);
        seq_t cand = prev + 1;
        while (name_contains_letter(cand, forbid))
            ++cand;
        cache[i] = cand;
    }
    cache_sz = n;
    return cache[n];
}
