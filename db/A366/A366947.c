/*
 * A366947  Lexicographically earliest sequence of distinct terms > 0 such
 *          that any digit d jumping to the right over d digits lands on a
 *          nonsquare digit.
 * https://oeis.org/A366947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: lexicographically earliest sequence of distinct
   positive integers such that every digit d jumping to the right
   over d digits lands on a nonsquare decimal digit (2,3,5,6,7,8).
   The algorithm builds the sequence greedily, keeping a global
   list of terms and the concatenated digit stream.  Each candidate
   is tested for the jump condition using only the last at most
   ten digits of the stream and the digits of the candidate itself. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Non‑square digits in decimal */
static bool const non_square[10] = {
    false, /* 0 */
    false, /* 1 */
    true,  /* 2 */
    true,  /* 3 */
    false, /* 4 */
    true,  /* 5 */
    true,  /* 6 */
    true,  /* 7 */
    true,  /* 8 */
    false  /* 9 */
};

/* Simple dynamic array helpers for seq_t and for the digit stream */
static seq_t *terms = NULL;      /* list of sequence terms */
static size_t terms_len = 0, terms_cap = 0;

static unsigned char *stream = NULL; /* concatenated decimal digits of all terms so far */
static size_t stream_len = 0, stream_cap = 0;

/* Ensure capacity for the terms array */
static void ensure_terms_cap(size_t need)
{
    if (terms_cap < need) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 4;
        if (new_cap < need) new_cap = need;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
}

/* Ensure capacity for the digit stream array */
static void ensure_stream_cap(size_t need)
{
    if (stream_cap < need) {
        size_t new_cap = stream_cap ? stream_cap * 2 : 8;
        if (new_cap < need) new_cap = need;
        stream = (unsigned char *)realloc(stream, new_cap * sizeof(unsigned char));
        stream_cap = new_cap;
    }
}

/* Return true iff x already appears in the sequence */
static bool already_used(seq_t x)
{
    for (size_t i = 0; i < terms_len; ++i)
        if (terms[i] == x) return true;
    return false;
}

/* Convert a seq_t value to its decimal digits (most significant first).
   The result is stored into the supplied buffer 'digits' (must have
   enough space), and the function returns the number of digits. */
static size_t digits_of_seq(seq_t val, unsigned char *digits)
{
    size_t pos = 0;
    if (val == 0) {
        digits[pos++] = 0;
    } else {
        unsigned char tmp[32];    /* val will have at most 32 digits for n=10000 */
        size_t tpos = 0;
        while (val > 0) {
            unsigned dig = (unsigned)((val % 10) > 0 ? (val % 10) : (val % 10));
            tmp[tpos++] = (unsigned char)dig;
            val = val / 10;
        }
        /* reverse into digits[] (most significant first) */
        for (int i = (int)tpos - 1; i >= 0; --i)
            digits[pos++] = tmp[i];
    }
    return pos;
}

/* Main sequence function.  Builds the sequence lazily and returns
   a(n).  Keeps the global state across calls. */
seq_t a(idx_t n)
{
    /* Build until we have at least n terms */
    while (terms_len < (size_t)n) {
        seq_t cand = (seq_t)1;
        /* Find the smallest unused positive integer that satisfies the rule */
        for (;;) {
            if (already_used(cand)) { ++cand; continue; }

            /* Get digits of candidate */
            unsigned char cand_digits[32];
            size_t m = digits_of_seq(cand, cand_digits); /* m = number of digits */

            bool ok = true;
            size_t L = stream_len;                      /* current stream length */

            /* Check jumps from the last up to 10 digits of the stream to the new segment */
            size_t start = (L >= 10) ? L - 10 : 0;
            for (size_t i = start; i < L && ok; ++i) {
                unsigned char d = stream[i];
                size_t landing = i + d + 1;
                if (landing >= L && landing < L + m) {
                    unsigned char target = cand_digits[landing - L];
                    if (!non_square[target]) { ok = false; break; }
                }
            }

            /* Check jumps from digits of the new term that land inside the new segment */
            if (ok) {
                for (size_t j = 0; j < m && ok; ++j) {
                    unsigned char d = cand_digits[j];
                    size_t landing = L + j + d + 1;
                    if (landing < L + m) { /* lands inside new segment */
                        unsigned char target = cand_digits[landing - L];
                        if (!non_square[target]) { ok = false; break; }
                    }
                }
            }

            if (ok) {
                /* Accept this candidate */
                ensure_terms_cap(terms_len + 1);
                terms[terms_len++] = cand;
                ensure_stream_cap(stream_len + m);
                for (size_t k = 0; k < m; ++k)
                    stream[stream_len + k] = cand_digits[k];
                stream_len += m;
                break;   /* move to next term */
            }
            ++cand;
        }
    }
    return terms[(size_t)n - 1];
}
