/*
 * A349887  Numbers which, when written in English, contain exactly one
 *          instance of the letter E.
 * https://oeis.org/A349887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose English name contains exactly one instance of the letter E.
   The function sequentially enumerates such numbers and caches them.
   Implementation notes:
   - The English spelling is constructed from units (0‑9), teens (10‑19),
     tens (20‑90), and the hundreds word.  "hundred" contains one E,
     while the scale words "thousand", "million", … contain none.
   - The count of 'E' in numbers 0‑99 is pre‑computed with small tables.
   - A simple dynamic array caches the sequence values to enable arbitrary
     re‑ordering of calls.  The cache grows geometrically.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* E‑counts for 0‑9  (zero to nine) */
static const unsigned char unit_e[10] = {
    1, /* zero  */
    1, /* one   */
    0, /* two   */
    2, /* three */
    0, /* four  */
    1, /* five  */
    0, /* six   */
    2, /* seven */
    1, /* eight */
    1  /* nine  */
};

/* E‑counts for 10‑19 */
static const unsigned char teen_e[10] = {
    1, /* ten      */
    3, /* eleven   */
    2, /* twelve   */
    2, /* thirteen */
    2, /* fourteen */
    2, /* fifteen  */
    2, /* sixteen  */
    4, /* seventeen */
    3, /* eighteen */
    3  /* nineteen */
};

/* E‑counts for tens 20,30,…,90 (index = tens digit) */
static const unsigned char tens_e[10] = {
    0, /* 0 – not used */
    0, /* 1 – not used */
    1, /* 2 – twenty   */
    0, /* 3 – thirty   */
    0, /* 4 – forty    */
    0, /* 5 – fifty    */
    0, /* 6 – sixty    */
    2, /* 7 – seventy  */
    1, /* 8 – eighty   */
    1  /* 9 – ninety   */
};

static const unsigned char e_hundred = 1; /* "hundred" */

/* Count the number of 'E's in the English name of n (0 inclusive). */
static unsigned int count_e(idx_t n)
{
    if (n == 0) return unit_e[0]; /* "zero" */

    unsigned int cnt = 0;
    idx_t num = n;

    /* Process each 3‑digit group (units, thousands, millions, …) */
    while (num > 0) {
        int g = (int)(num % 1000);   /* current group */
        if (g) {
            int h = g / 100;         /* hundreds digit */
            int t = g % 100;         /* last two digits */

            /* hundreds part: e.g., "two hundred"  */
            if (h > 0) {
                cnt += unit_e[h] + e_hundred;
            }

            /* tens and units part */
            if (t == 0) {
                /* nothing to add */
            } else if (t < 10) {
                cnt += unit_e[t];
            } else if (t < 20) {
                cnt += teen_e[t - 10];
            } else {
                int tens = t / 10;
                int units = t % 10;
                cnt += tens_e[tens];
                if (units > 0) cnt += unit_e[units];
            }
            /* scale words (thousand, million, …) contain no 'E', skip */
        }
        num /= 1000;
    }
    return cnt;
}

/* Simple dynamic array for caching sequence values (seq_t). */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_candidate = 0;

static void grow_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t newcap = cache_cap ? cache_cap * 2 : 16;
        while (newcap < needed) newcap *= 2;
        seq_cache = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }
}

/* Return the n‑th term (a(1) = 0). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, return 0 */

    /* Return cached value if it already exists. */
    if ((size_t)n <= cache_len) {
        return seq_cache[(size_t)n - 1];
    }

    /* Generate terms until we have n of them. */
    while (cache_len < (size_t)n) {
        while (1) {
            /* Find the next number whose English name has exactly one 'E'. */
            if (count_e(next_candidate) == 1) {
                grow_cache(cache_len + 1);
                seq_cache[cache_len++] = (seq_t)next_candidate;
                next_candidate++;          /* advance to avoid duplicate */
                break;
            }
            next_candidate++;              /* try next candidate */
        }
    }

    return seq_cache[(size_t)n - 1];
}
