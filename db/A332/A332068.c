/*
 * A332068  Numbers whose English name has at least two vowels and all the
 *          vowels are in alphabetical order.
 * https://oeis.org/A332068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   check_word
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search approach: iterate natural numbers, keep those whose English names
 * have at least two vowels (a,e,i,o,u) and all vowels in non‑decreasing
 * alphabetical order.  The sequence is memoised so any order of calls works. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/check_word.h"

static void write_three(idx_t n, char **p);
static void number_to_words(idx_t n, char *buf);
static bool check_word(const char *s);
static bool check_number(idx_t n);

/* ---- word components ---------------------------------------------------- */
static const char *const units[20] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen",
    "eighteen","nineteen"};
static const char *const tens_words[10] = {
    "","", "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"};
static const idx_t   group_vals[4] = {1000000000000ULL, 1000000000ULL,
                                      1000000ULL, 1000ULL};
static const char  *const group_names[4] = {"trillion","billion",
                                            "million","thousand"};

/* ---- convert a number to its English words (lowercase) ----------------- */
static void write_three(idx_t n, char **p)
{
    idx_t h = n / 100;
    idx_t t = n % 100;

    if (h) {
        const char *w = units[h];
        while (*w) (*p)[0] = *w, ++(*p), ++w;
        (*p)[0] = ' ';
        ++(*p);
        const char *hu = "hundred";
        while (*hu) (*p)[0] = *hu, ++(*p), ++hu;
    }

    if (t) {
        if (h) (*p)[0] = ' ', ++(*p);
        if (t < 20) {
            const char *w = units[t];
            while (*w) (*p)[0] = *w, ++(*p), ++w;
        } else {
            idx_t ten = t / 10;
            const char *w = tens_words[ten];
            while (*w) (*p)[0] = *w, ++(*p), ++w;
            if (t % 10) {
                (*p)[0] = ' ', ++(*p);
                w = units[t % 10];
                while (*w) (*p)[0] = *w, ++(*p), ++w;
            }
        }
    }
}

static void number_to_words(idx_t n, char *buf)
{
    if (n == 0) {
        strcpy(buf, "zero");
        return;
    }
    char *p = buf;
    bool any = false;
    for (int i = 0; i < 4; ++i) {
        idx_t g = (n / group_vals[i]) % 1000;
        if (!g) continue;
        write_three(g, &p);
        *p++ = ' ';
        const char *name = group_names[i];
        while (*name) *p++ = *name++;
        *p++ = ' ';
        any = true;
    }
    idx_t rest = n % 1000;
    if (rest) {
        write_three(rest, &p);
        any = true;
    }
    if (!any) {
        *p++ = '0';
    }
    if (p > buf && *(p - 1) == ' ') *(p - 1) = '\0';
    else *p = '\0';
}

static bool check_number(idx_t n)
{
    static char buf[512];
    number_to_words(n, buf);
    return check_word(buf);
}

/* ---- sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t  *seq  = NULL;
    static idx_t  seq_cap = 0;
    static idx_t  seq_len = 0;
    static idx_t  next_candidate = 0;

    if (n == 0) return 0;                    /* safety for offset handling */

    if (seq_len >= n) return seq[n - 1];

    /* Ensure array capacity */
    size_t needed = n;
    if (needed > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* Find the next candidates */
    while (seq_len < n) {
        while (!check_number(next_candidate)) ++next_candidate;
        seq[seq_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return seq[n - 1];
}
