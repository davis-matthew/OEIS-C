/*
 * A332069  Numbers whose American English name contains all 5 vowels in
 *          alphabetical order.
 * https://oeis.org/A332069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A332069: numbers whose American English name contains all 5 vowels
   a, e, i, o, u in that order as a subsequence.
   The function searches numbers incrementally and caches found
   terms so that calls in arbitrary order are efficient. */

#include <stdlib.h>
#include <stddef.h>

static const char VOWELS[] = "aeiou";

/* Advance the vowel scan whenever the current vowel is found. */
static void add_word(const char *w, int *need)
{
    for (const char *p = w; *p; ++p) {
        if (*need < 5 && *p == VOWELS[*need])
            ++(*need);
    }
}

/* English word tables */
static const char *ONES[]  = {"", "one", "two", "three", "four",
                              "five", "six", "seven", "eight", "nine"};
static const char *TEENS[] = {"ten", "eleven", "twelve",
                               "thirteen", "fourteen", "fifteen",
                               "sixteen", "seventeen", "eighteen", "nineteen"};
static const char *TENS[]  = {"", "", "twenty", "thirty", "forty",
                               "fifty", "sixty", "seventy", "eighty", "ninety"};
static const char *SCALE[] = {"", "thousand", "million", "billion",
                               "trillion", "quadrillion", "quintillion",
                               "sextillion", "septillion", "octillion",
                               "nonillion", "decillion", "undecillion",
                               "duodecillion"};

/* Convert a three‑digit group into words and update vowel scan. */
static void process_group(int val, int scale_idx, int *need)
{
    if (val == 0) return;

    int hundred = val / 100;
    int rem = val % 100;

    if (hundred) {
        add_word(ONES[hundred], need);
        add_word("hundred", need);
    }

    if (rem) {
        if (rem < 20) {
            if (rem < 10)
                add_word(ONES[rem], need);
            else
                add_word(TEENS[rem-10], need);
        } else {
            int t = rem / 10;
            int u = rem % 10;
            add_word(TENS[t], need);
            if (u)
                add_word(ONES[u], need);
        }
    }

    if (scale_idx > 0)
        add_word(SCALE[scale_idx], need);
}

/* Return 1 if the American English name of n contains a,e,i,o,u
   in that order, otherwise 0. */
static int has_vowel_order(seq_t n)
{
    if (n == 0) return 0;

    int groups[64];
    size_t gcnt = 0;
    seq_t temp = n;
    const seq_t THOUSAND = (seq_t)1000;
    while (temp > 0) {
        int val = (int)(temp % THOUSAND);
        temp /= THOUSAND;
        groups[gcnt++] = val;
        if (gcnt == sizeof(groups)/sizeof(groups[0]))
            break;
    }

    int need = 0;
    for (int gi = (int)gcnt - 1; gi >= 0; --gi) {
        if (groups[gi] == 0) continue;
        process_group(groups[gi], gi, &need);
        if (need == 5) return 1;
    }
    return 0;
}

/* ---------- cache ------------------------------------ */
#define INIT_CAP 128
static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t count = 0;
static seq_t next_search = (seq_t)1;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined below offset */

    while (count < n) {
        while (!has_vowel_order(next_search))
            ++next_search;

        if (count == cap) {
            size_t newcap = (cap == 0) ? INIT_CAP : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache = tmp;
            cap = newcap;
        }

        cache[count++] = next_search;
        ++next_search;
    }
    return cache[n-1];
}
