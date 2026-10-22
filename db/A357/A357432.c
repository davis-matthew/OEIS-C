/*
 * A357432  a(1) = 1; a(2) = 2; for n > 2, a(n) is the smallest positive
 *          number not occurring earlier such that a(n) plus the sum of all
 *          previous terms appears in the string concatenation of
 *          a(1)..a(n-1).
 * https://oeis.org/A357432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A357432 sequence implementation.
 *
 * a(1) = 1; a(2) = 2;
 * For n > 2, a(n) is the smallest positive integer not occurring earlier
 * such that a(n) + sum_{k=1}^{n-1} a(k) appears as a substring in the
 * decimal concatenation of a(1)..a(n-1).
 *
 * The function works for any seq_t / idx_t defined by the build; it uses
 * only integer arithmetic, a few standard library routines, and the
 * provided seq_to_str helper.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/seq_to_str.h"

/* cached values ---------------------------------------------------------- */
static seq_t *cache = NULL;   /* sequence values */
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* used values storage (for mex) ---------------------------------------- */
static seq_t *used_vals = NULL;   /* appended in order as they appear */
static size_t used_cap = 0;
static size_t used_len = 0;

/* running sum of all terms so far ---------------------------------------- */
static seq_t sum_prev = 0;

/* the smallest positive integer not yet seen --------------------------- */
static seq_t mex = 1;

/* concatenation of all terms in decimal form --------------------------- */
static char *concat_str = NULL;
static size_t concat_cap = 0;
static size_t concat_len = 0;

/* ---------- helper functions ------------------------------------------- */

static inline void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

static inline void ensure_used_cap(size_t need)
{
    if (need <= used_cap) return;
    size_t new_cap = used_cap ? (used_cap << 1) : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(used_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    used_vals = tmp;
    used_cap = new_cap;
}

static inline void ensure_concat_cap(size_t need)
{
    /* need = number of digits to hold (no NUL yet) */
    if (need <= concat_cap) return;
    size_t new_cap = concat_cap ? (concat_cap << 1) : 64;
    while (new_cap < need) new_cap <<= 1;
    char *tmp = (char *)realloc(concat_str, new_cap);
    if (!tmp) abort();
    concat_str = tmp;
    concat_cap = new_cap;
}

/* return decimal representation of v in buf (no NUL), and write length
 * into *len.  Caller may provide a buffer large enough (256 bytes here). */
static inline size_t seqt_to_cstr(seq_t v, char *buf, size_t buflen)
{
    size_t len;
    seq_to_str(v, buf, &len);
    /* ensure NUL for strcmp/strstr usage */
    if (len < buflen) buf[len] = '\0';
    else if (buflen > 0) buf[buflen - 1] = '\0';
    return len;
}

/* check whether a value is already present in used_vals (linear search). */
static inline bool used_contains(seq_t x)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used_vals[i] == x) return true;
    return false;
}

/* advance mex to the next unused positive integer. */
static inline void advance_mex(void)
{
    while (used_contains(mex)) ++mex;
}

/* ----------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) abort();     /* sequence is 1‑indexed */

    /* lazy initialization on first call --------------------------------- */
    if (cache_len == 0)
    {
        /* reserve space for first two terms */
        ensure_cache_cap(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = 2;

        /* store used values */
        ensure_used_cap(2);
        used_vals[0] = (seq_t)1;
        used_vals[1] = (seq_t)2;
        used_len = 2;

        mex = (seq_t)3;

        sum_prev = (seq_t)1 + (seq_t)2;

        /* initial concatenation "12" */
        ensure_concat_cap(2);
        concat_str[0] = '1';
        concat_str[1] = '2';
        concat_len = 2;
        concat_str[concat_len] = '\0';
    }

    /* generate terms until the requested index is reached ---------------- */
    while (cache_len < (size_t)n)
    {
        seq_t cand = mex;
        for (;; ++cand)
        {
            if (used_contains(cand)) continue;  /* already used */

            seq_t t = sum_prev + cand;
            char tbuf[256];
            seqt_to_cstr(t, tbuf, sizeof(tbuf));

            if (strstr(concat_str, tbuf) != NULL)   /* substring exists */
            {
                /* accept cand as the next sequence value */

                /* append to cache */
                ensure_cache_cap(cache_len + 1);
                cache[cache_len] = cand;
                ++cache_len;

                /* record usage */
                ensure_used_cap(used_len + 1);
                used_vals[used_len] = cand;
                ++used_len;

                /* update mex */
                advance_mex();

                /* update running sum */
                sum_prev = sum_prev + cand;

                /* append cand to concatenation */
                char numbuf[256];
                seqt_to_cstr(cand, numbuf, sizeof(numbuf));
                size_t numlen = strlen(numbuf);
                ensure_concat_cap(concat_len + numlen);
                memcpy(concat_str + concat_len, numbuf, numlen);
                concat_len += numlen;
                concat_str[concat_len] = '\0';

                break;  /* next term computed */
            }
            /* else, find next unused candidate */
        }
    }

    return cache[(size_t)n - 1];
}
