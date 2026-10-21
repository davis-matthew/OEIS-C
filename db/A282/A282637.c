/*
 * A282637  a(1) = 1. a(n) is the smallest unlisted natural number whose name
 *          in (American) English ends with the letter a(n-1) starts with, and
 *          makes the sequence extendable.
 * https://oeis.org/A282637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A282637:
   a(1) = 1. For n > 1, a(n) is the smallest positive integer not
   yet used whose English name ends with the first letter of the
   English name of a(n‑1) and whose name does not begin with the
   letters 'f' or 's'.  The sequence is built incrementally, caching
   already computed terms.  The implementation keeps an array of
   booleans indexed by the numeric value (cast to unsigned long long),
   which is sufficient for the expected size of the sequence (the
   first 10 000 terms comfortably fit in 64 bits).  */

/* needs to compile under C11 and C++17.  */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*----------------------------------------------------------------------*/
/*  English number → words   (uint64_t only).  */
/*----------------------------------------------------------------------*/
static void append_word(char *out, size_t *pos, const char *word)
{
    size_t wlen = strlen(word);
    memcpy(out + *pos, word, wlen);
    *pos += wlen;
    out[*pos] = ' ';
    (*pos)++;
}

static void number_to_words(uint64_t n, char *out)
{
    static const char *units[] = {"", "one", "two", "three", "four",
                                  "five", "six", "seven", "eight", "nine"};
    static const char *teens[] = {"ten", "eleven", "twelve", "thirteen", "fourteen",
                                  "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
    static const char *tens[]  = {"", "", "twenty", "thirty", "forty",
                                  "fifty", "sixty", "seventy", "eighty", "ninety"};
    static const char *scales[] = {"thousand", "million", "billion", "trillion",
                                   "quadrillion", "quintillion", "sextillion",
                                   "septillion", "octillion", "nonillion", "decillion"};

    size_t pos = 0;
    if (n == 0) {
        strcpy(out, "zero");
        return;
    }
    uint64_t groups[12];
    int gcnt = 0;
    uint64_t rem = n;
    while (rem > 0 && gcnt < 12) {
        groups[gcnt++] = rem % 1000;
        rem /= 1000;
    }
    for (int k = gcnt - 1; k >= 0; --k) {
        uint64_t g = groups[k];
        if (g == 0)
            continue;

        if (g >= 100) {
            uint64_t h = g / 100;
            append_word(out, &pos, units[h]);
            append_word(out, &pos, "hundred");
            g %= 100;
        }
        if (g >= 20) {
            uint64_t t = g / 10;
            append_word(out, &pos, tens[t]);
            g %= 10;
        }
        if (g >= 10) {
            append_word(out, &pos, teens[g - 10]);
        } else if (g > 0) {
            append_word(out, &pos, units[g]);
        }
        if (k > 0) {
            append_word(out, &pos, scales[k - 1]);
        }
    }
    /* Remove trailing space */
    if (pos > 0 && out[pos - 1] == ' ')
        pos--;
    out[pos] = '\0';
}

static void get_first_last(uint64_t n, char *first, char *last)
{
    char buf[512];
    number_to_words(n, buf);
    *first = buf[0];
    size_t len = strlen(buf);
    size_t i = len;
    while (i > 0 && buf[i - 1] == ' ')
        i--;
    *last = buf[i - 1];
}

/*----------------------------------------------------------------------*/
/*  State for memoised sequence.  */
/*----------------------------------------------------------------------*/
static idx_t seq_len = 0;          /* number of terms stored, 0 if none yet */
static seq_t *seq_arr = NULL;     /* array of terms  */
static size_t seq_cap = 0;        /* capacity of seq_arr    */

static bool *used = NULL;         /* used[n] is true if number n already used  */
static size_t used_cap = 0;       /* capacity of used[]  */
static char *first_letters = NULL;/* first_letters[n] is first letter of n */
static char *last_letters  = NULL;/* last_letters[n]  is last letter of n  */

/* allocate enough space for seq_arr */
static void ensure_seq_cap(size_t min_cap)
{
    if (seq_cap >= min_cap)
        return;
    size_t new_cap = seq_cap == 0 ? 64 : seq_cap;
    while (new_cap < min_cap)
        new_cap <<= 1;
    /* C++ requires cast, C handles void*. */
    seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_arr = tmp;
    seq_cap = new_cap;
}

/* allocate enough space for used[], first_letters[], last_letters[] */
static void ensure_used_capacity(size_t max_index)
{
    if (used_cap >= max_index)
        return;
    size_t new_cap = used_cap == 0 ? 128 : used_cap;
    while (new_cap < max_index)
        new_cap <<= 1;
    bool *tmpU = (bool *)realloc(used, (new_cap + 1) * sizeof(bool));
    char *tmpF = (char *)realloc(first_letters, (new_cap + 1) * sizeof(char));
    char *tmpL = (char *)realloc(last_letters, (new_cap + 1) * sizeof(char));
    if (!tmpU || !tmpF || !tmpL) abort();
    /* initialise new portions */
    for (size_t i = used_cap + 1; i <= new_cap; ++i) {
        tmpU[i] = false;
        tmpF[i] = 0;
        tmpL[i] = 0;
    }
    used = tmpU;
    first_letters = tmpF;
    last_letters  = tmpL;
    used_cap = new_cap;
}

/* cache the first and last letters of a number (cast to uint64_t) */
static void cache_first_last(uint64_t v)
{
    if (v > used_cap)
        ensure_used_capacity(v);
    if (first_letters[v] == 0) {
        char f, l;
        get_first_last(v, &f, &l);
        first_letters[v] = f;
        last_letters[v]  = l;
    }
}

/*---------------------------------------------------------------------*/
/*  Public entry point.                                               */
/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but never requested */

    /* initialise with first term if needed */
    if (seq_len == 0) {
        ensure_seq_cap(1);
        seq_arr[0] = (seq_t)1;
        seq_len = 1;
        ensure_used_capacity(1);
        used[1] = true;
        cache_first_last(1);
    }

    /* generate terms up to n */
    if (seq_len < n) {
        ensure_seq_cap(n);
        for (idx_t idx = seq_len + 1; idx <= n; ++idx) {
            /* previous term */
            seq_t prev = seq_arr[idx - 2];  /* a(n-1) at this point */
            uint64_t prev_int = (uint64_t)prev;
            cache_first_last(prev_int);
            char required = first_letters[prev_int];

            /* search candidate */
            idx_t cand = 1;
            while (1) {
                uint64_t cand_int = (uint64_t)cand;
                if (cand_int > used_cap)
                    ensure_used_capacity(cand_int);
                cache_first_last(cand_int);
                if (!used[cand_int] &&
                    last_letters[cand_int] == required &&
                    first_letters[cand_int] != 'f' &&
                    first_letters[cand_int] != 's') {
                    used[cand_int] = true;
                    seq_arr[seq_len++] = (seq_t)cand;
                    break;
                }
                ++cand;
            }
        }
    }
    return seq_arr[n - 1];
}
