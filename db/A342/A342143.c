/*
 * A342143  Take a(n), sort its digits into ascending order, divide the larger
 *          of the two numbers by the smaller and keep only the remainder:
 *          this remainder is present in a(n) as a substring of its digits.
 * https://oeis.org/A342143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a lexicographically earliest distinct positive integer
 *            whose * remainder from dividing the larger of the number and its
 *            digits sorted in * ascending order by the smaller is a substring
 *            of the number. * * The function is memoized so that repeated
 *            calls with arbitrary n work * efficiently
 * Range:     u8 n <= 33, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   digit_len, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/get_digits.h"   /* void get_digits(seq_t x, uint8_t *buf, size_t *len) */
#include "../utils/digit_len.h"   /* int digit_len(seq_t x) */

/* maximum digits we expect for any seq_t value used in the sequence.
 * 512 is more than enough for 128‑bit integers (39 digits) or even for
 * larger arbitrary precision numbers encountered for n up to 10 000. */
#define MAX_DIGITS 512

static seq_t *cache = NULL;        /* array of generated terms */
static size_t cache_size = 0;      /* how many terms we have stored */
static size_t cache_cap  = 0;      /* allocated capacity */
static seq_t  candidate  = 10;     /* next integer to test */

/* Test whether integer x satisfies the OEIS condition. */
static bool is_good(seq_t x)
{
    uint8_t orig[MAX_DIGITS];
    uint8_t sorted[MAX_DIGITS];
    uint8_t rdigits[MAX_DIGITS];
    size_t  xlen, rlen;
    size_t  i, j;

    /* get decimal digits of x (most significant first) */
    get_digits(x, orig, &xlen);

    /* copy for sorting */
    for (i = 0; i < xlen; ++i) sorted[i] = orig[i];

    /* insertion sort ascending */
    for (i = 1; i < xlen; ++i) {
        uint8_t key = sorted[i];
        j = i;
        while (j > 0 && sorted[j - 1] > key) {
            sorted[j] = sorted[j - 1];
            --j;
        }
        sorted[j] = key;
    }

    /* build the sorted value (strip leading zeros) */
    seq_t sorted_val = 0;
    i = 0;
    while (i < xlen && sorted[i] == 0) ++i;
    for (; i < xlen; ++i) sorted_val = sorted_val * 10 + sorted[i];

    /* larger and smaller numbers for remainder */
    seq_t larger  = (x > sorted_val) ? x : sorted_val;
    seq_t smaller = (x < sorted_val) ? x : sorted_val;
    seq_t rem     = larger % smaller;

    /* digits of remainder */
    get_digits(rem, rdigits, &rlen);

    /* substring test: does rdigits appear in orig? */
    if (rlen > xlen) return false;
    for (i = 0; i + rlen <= xlen; ++i) {
        if (memcmp(&orig[i], rdigits, rlen) == 0) return true;
    }
    return false;
}

/* Ensure that at least 'target' terms are stored in cache. */
static void ensure_terms(idx_t target)
{
    while (cache_size < target) {
        if (is_good(candidate)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { perror("realloc"); exit(EXIT_FAILURE); }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; return 0 for invalid request */
    ensure_terms(n);
    return cache[n - 1];
}
