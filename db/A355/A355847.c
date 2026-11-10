/*
 * A355847  Irregular table read by rows, in which the rows list integers
 *          formed in the process in A180301, but generalized to other
 *          starting integers. A row ends when reaching a term in A180301.
 * https://oeis.org/A355847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, word
 * Formula:   a(n) = n-th term of the table whose row k lists k, then
 *            repeatedly the next larger integer whose US-English spelling
 *            (hyphenated) sorts lexicographically later, ending at the first
 *            term that belongs to A180301 (computed by direct search)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static const char *A355847_ones[20] = {"", "one", "two", "three", "four", "five", "six", "seven",
    "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen",
    "sixteen", "seventeen", "eighteen", "nineteen"};
static const char *A355847_tens[10] = {"", "", "twenty", "thirty", "forty", "fifty",
    "sixty", "seventy", "eighty", "ninety"};
static const char *A355847_scale[7] = {"", "thousand", "million", "billion", "trillion",
    "quadrillion", "quintillion"};

static void A355847_add(char *b, const char *s)
{
    if (b[0]) strcat(b, " ");
    strcat(b, s);
}

static void A355847_small(char *b, unsigned v)
{
    if (v >= 100) {
        A355847_add(b, A355847_ones[v / 100]);
        A355847_add(b, "hundred");
        v %= 100;
    }
    if (v >= 20) {
        char w[40];
        strcpy(w, A355847_tens[v / 10]);
        if (v % 10) {
            strcat(w, "-");
            strcat(w, A355847_ones[v % 10]);
        }
        A355847_add(b, w);
    } else if (v > 0) {
        A355847_add(b, A355847_ones[v]);
    }
}

static void A355847_spell(uint64_t x, char *b)
{
    b[0] = 0;
    if (x == 0) { strcpy(b, "zero"); return; }
    uint64_t p[7];
    p[0] = 1;
    for (int i = 1; i < 7; i++) p[i] = p[i - 1] * 1000ULL;
    for (int i = 6; i >= 0; i--) {
        unsigned chunk = (unsigned)((x / p[i]) % 1000ULL);
        if (chunk) {
            A355847_small(b, chunk);
            if (i) A355847_add(b, A355847_scale[i]);
        }
    }
}

/* membership in A180301 (values below 2*10^36 and fitting uint64) */
static bool A355847_inA(uint64_t x)
{
    static const uint64_t base[11] = {1, 2, 200, 201, 202, 2000, 2001, 2002, 2200, 2201, 2202};
    const uint64_t P = 2000000000000ULL;
    if (x >= P) {
        if (x == P) return true;
        x -= P;
    }
    for (int i = 0; i < 11; i++)
        if (base[i] == x) return true;
    return false;
}

static uint64_t A355847_next(uint64_t m)
{
    char s[512], t[512];
    A355847_spell(m, s);
    uint64_t y = m + 1;
    for (;; y++) {
        A355847_spell(y, t);
        if (strcmp(t, s) > 0) return y;
    }
}

static idx_t A355847_cache_k = 1;
static idx_t A355847_cache_start = 1;

seq_t a(idx_t n)
{
    idx_t k, start;
    if (A355847_cache_start <= n) {
        k = A355847_cache_k;
        start = A355847_cache_start;
    } else {
        k = 1;
        start = 1;
    }
    for (;;) {
        uint64_t x = (uint64_t)k;
        idx_t pos = start;
        for (;;) {
            if (pos == n) {
                A355847_cache_k = k;
                A355847_cache_start = start;
                return (seq_t)x;
            }
            if (A355847_inA(x)) break;
            x = A355847_next(x);
            pos++;
        }
        start = pos + 1;
        k++;
    }
}
