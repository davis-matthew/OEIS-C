/*
 * A335497  a(1) = 1, and for any n > 0, a(n+1) is the number of times the
 *          decimal representation of a(n) appears in the concatenation of the
 *          first n terms, possibly with overlap.
 * https://oeis.org/A335497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 826, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1,
   a(n)=#occurrences of decimal representation of a(n-1)
   in the concatenation of a(1)…a(n-1), allowing overlaps. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/get_digits.h"      /* provides get_digits() */

static void seq_to_str(seq_t x, char *buf, size_t *len)
{
    /* Convert an arbitrary-precision seq_t to a decimal string.
       get_digits() supplies the digits least‑significant first.
       We reverse them to obtain the usual most‑significant first. */
    uint8_t digits[512];
    size_t d;
    get_digits(x, digits, &d);          /* d digits, least‑significant first */
    *len = d;
    for (size_t i = 0; i < d; ++i)
        buf[i] = (char)('0' + digits[d - 1 - i]); /* reverse order */
}

/* Count overlapping occurrences of pat[0..pat_len-1] in str[0..str_len-1] */
static size_t count_occurrences(const char *str, size_t str_len,
                                const char *pat, size_t pat_len)
{
    if (pat_len == 0 || pat_len > str_len)
        return 0;
    size_t count = 0;
    for (size_t i = 0; i + pat_len <= str_len; ++i) {
        int ok = 1;
        for (size_t j = 0; j < pat_len; ++j) {
            if (str[i + j] != pat[j]) {
                ok = 0;
                break;
            }
        }
        if (ok)
            ++count;
    }
    return count;
}

/* Memoised storage of terms and the running concatenation. */
static seq_t *terms = NULL;          /* cached terms */
static idx_t terms_cap = 0, terms_len = 0;

static char *concat = NULL;
static size_t concat_cap = 0, concat_len = 0;

static void ensure_terms_cap(idx_t need)
{
    if (terms_cap >= need)
        return;
    idx_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

static void ensure_concat_cap(size_t extra)
{
    if (concat_len + extra <= concat_cap)
        return;
    size_t new_cap = concat_cap ? concat_cap * 2 : 64;
    while (new_cap < concat_len + extra)
        new_cap *= 2;
    concat = (char *)realloc(concat, new_cap * sizeof(char));
    concat_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;   /* undefined for n=0, but return 0 safely */

    if (terms_len < n) { /* compute missing terms on demand */
        ensure_terms_cap(n);

        for (idx_t i = terms_len; i < n; ++i) {
            if (i == 0) {            /* a(1) = 1 */
                terms[0] = (seq_t)1;
                char buf[64];
                size_t blen;
                seq_to_str(terms[0], buf, &blen);
                ensure_concat_cap(blen);
                for (size_t k = 0; k < blen; ++k)
                    concat[concat_len + k] = buf[k];
                concat_len += blen;
            } else {
                /* Count occurrences of a(i) in current concatenation. */
                char pat[64];
                size_t pat_len;
                seq_to_str(terms[i - 1], pat, &pat_len);
                size_t cnt = count_occurrences(concat, concat_len, pat, pat_len);
                terms[i] = (seq_t)cnt;

                /* Append the decimal representation of a(i) */
                char out[64];
                size_t out_len;
                seq_to_str(terms[i], out, &out_len);
                ensure_concat_cap(out_len);
                for (size_t k = 0; k < out_len; ++k)
                    concat[concat_len + k] = out[k];
                concat_len += out_len;
            }
        }
        terms_len = n;
    }
    return terms[n - 1];
}
