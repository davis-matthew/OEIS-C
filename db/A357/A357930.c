/*
 * A357930  a(0) = 0; for n > 0, let S = concatenation of a(0)..a(n-1); a(n)
 *          is the number of times the digit at a(n-1) digits back from the
 *          end of S appears in S.
 * https://oeis.org/A357930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 737, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 107 terms through n = 9676 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n>0 let S = concat a(0)..a(n-1);
   a(n) = number of occurrences of digit S[|S|-1-a(n-1)] in S */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

seq_t a(idx_t n) {
    /* cache of computed a(i) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;        /* current allocated size */
    static size_t next  = 0;        /* next index to compute   */

    /* concatenated decimal representation of the sequence */
    static char *Sdigits = NULL;
    static size_t Slen   = 0;        /* current used length   */
    static size_t Scap   = 0;        /* current allocated cap */

    /* frequency of digits 0‑9 in S, stored as seq_t */
    static seq_t freq[10] = {0};

    /* grow cache lazily */
    while (next <= (size_t)n) {
        /* ensure the cache array is large enough */
        if (cap <= next) {
            size_t new_cap = (cap == 0) ? 1 : cap * 2;
            if (new_cap <= cap)          /* avoid overflow */
                new_cap = (size_t)n + 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();
            cache = tmp;
            cap = new_cap;
        }

        /* initialise a(0) and the string S on first iteration */
        if (next == 0) {
            cache[0] = (seq_t)0;

            /* initialise S with character '0' */
            if (Scap == 0) {
                Scap    = 1;
                Sdigits = (char *)malloc(Scap);
                if (!Sdigits) abort();
            }
            Sdigits[0] = '0';
            Slen       = 1;
            freq[0] = (seq_t)1;
            next++;
            continue;
        }

        /* compute a(k) where k = next */
        size_t k       = next;
        seq_t a_prev   = cache[k - 1];

        /* index of the digit to inspect: |S|-1-a_prev */
        size_t pos = Slen - 1 - (size_t)a_prev;
        int digit  = Sdigits[pos] - '0';          /* guaranteed 0..9 */

        seq_t val = freq[digit];
        cache[k] = val;

        /* convert val to decimal string and append to S */
        char buf[128];
        size_t blen;
        seq_to_str(val, buf, &blen);

        /* make sure we have enough space for new digits */
        if (Slen + blen > Scap) {
            size_t new_cap = Scap ? Scap * 2 : 1;
            if (new_cap < Slen + blen) new_cap = Slen + blen;
            char *tmp = (char *)realloc(Sdigits, new_cap);
            if (!tmp) abort();
            Sdigits = tmp;
            Scap    = new_cap;
        }

        /* append digits and update frequency table */
        for (size_t i = 0; i < blen; ++i) {
            Sdigits[Slen + i] = buf[i];
            freq[buf[i] - '0'] = freq[buf[i] - '0'] + (seq_t)1;
        }
        Slen += blen;

        next++;
    }

    return cache[(size_t)n];
}
