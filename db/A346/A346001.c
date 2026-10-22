/*
 * A346001  Lexicographically earliest sequence of nonnegative integers such
 *          that two distinct terms differ by at least 5 decimal digits.
 * https://oeis.org/A346001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence of nonnegative integers such that
 * any two distinct terms differ by at least 5 decimal digits.
 * The sequence is generated greedily: a(1) = 0, and for n > 1 the
 * smallest integer greater than a(n‑1) that satisfies the 5‑digit
 * difference property with all previous terms is chosen. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uchar;

/* One term of the sequence: its value and decimal digits (least‑significant first). */
struct term {
    seq_t   value;
    uchar   digits[256];   /* enough for the expected size of the terms */
    size_t  len;           /* number of digits stored */
};

static struct term *terms     = NULL; /* array of generated terms          */
static size_t       terms_len = 0;     /* number of stored terms            */
static size_t       terms_cap = 0;     /* allocated capacity                */

static seq_t *cache     = NULL; /* cache of values for quick access   */
static size_t cache_len = 0;     /* number of cached values           */
static size_t cache_cap = 0;     /* allocated capacity                */

/* Ensure capacity for the terms array. */
static void ensure_terms_capacity(size_t needed)
{
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    if (new_cap < needed) new_cap = needed;
    struct term *tmp = (struct term *)realloc(terms, new_cap * sizeof(struct term));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* Ensure capacity for the cache array. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Convert a value to its decimal digits (least‑significant first). */
static void get_digits(seq_t x, unsigned char buf[256], size_t *lenp)
{
    size_t len = 0;
    seq_t tmp = x;
    do {
        buf[len++] = (unsigned char)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    } while (tmp != (seq_t)0);
    *lenp = len;
}

/* Test whether the candidate digit array differs in at least 5 positions
 * from all previously stored terms (with leading zeros for padding). */
static bool candidate_ok(const unsigned char *cand_digits, size_t cand_len)
{
    for (size_t i = 0; i < terms_len; ++i) {
        const struct term *t = &terms[i];
        size_t max_len = cand_len > t->len ? cand_len : t->len;
        size_t diff = 0;
        for (size_t pos = 0; pos < max_len; ++pos) {
            unsigned char d_c = (pos < cand_len)   ? cand_digits[pos] : 0;
            unsigned char d_t = (pos < t->len)     ? t->digits[pos] : 0;
            if (d_c != d_t) {
                ++diff;
                if (diff >= 5) break;
            }
        }
        if (diff < 5) return false;
    }
    return true;
}

/* External linkage: compute the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not part of the sequence, guard clause */

    /* Return directly if already computed. */
    if (cache_len >= (size_t)n) return cache[n - 1];

    /* Generate terms up to n. */
    for (seq_t new_index = (seq_t)cache_len + 1; new_index <= n; ++new_index) {
        seq_t cand;
        if (cache_len == 0) cand = (seq_t)0;
        else cand = cache[cache_len - 1] + (seq_t)1;

        unsigned char cand_digits[256];
        size_t cand_len = 0;
        get_digits(cand, cand_digits, &cand_len);

        /* Find the smallest candidate satisfying the distance constraint. */
        while (!candidate_ok(cand_digits, cand_len)) {
            ++cand;
            get_digits(cand, cand_digits, &cand_len);
        }

        /* Store in cache and detailed term data. */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len] = cand;

        ensure_terms_capacity(terms_len + 1);
        struct term *t = &terms[terms_len];
        t->value = cand;
        t->len   = cand_len;
        memcpy(t->digits, cand_digits, cand_len);

        ++terms_len;
        ++cache_len;
    }

    return cache[n - 1];
}
