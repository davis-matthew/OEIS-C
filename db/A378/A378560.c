/*
 * A378560  Numbers with the median of the digits equal to one of the digits.
 * https://oeis.org/A378560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers with median of the digits equal to one of the digits.
   The median is taken from the digits in ascending order.  All odd‑length
   decimal numbers satisfy the condition; for even‑length numbers the
   two middle digits must average to an integer m, and m itself must be
   a digit of the number.  This function returns the n‑th such number
   with offset 1 (a(1)=0).  It memoises generated terms so that calls
   in arbitrary order are O(n). */

/* The build system provides the integer types seq_t and idx_t. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/* Digit counting and property test                              */
/* ------------------------------------------------------------ */

static void digit_frequencies(seq_t x, unsigned char freq[10], size_t *len_out)
{
    if (x == 0) {
        /* only the digit 0 occurs once */
        freq[0] = 1;
        for (int d = 1; d < 10; ++d) freq[d] = 0;
        *len_out = 1;
        return;
    }
    const seq_t TEN = 10;
    unsigned char f[10] = {0};
    seq_t y = x;
    while (y != 0) {
        f[(int)(y % TEN)]++;
        y /= TEN;
    }
    size_t len = 0;
    for (int d = 0; d < 10; ++d) {
        freq[d] = f[d];
        len += f[d];
    }
    *len_out = len;
}

static bool has_property(seq_t x)
{
    unsigned char freq[10];
    size_t len;
    digit_frequencies(x, freq, &len);

    /* Odd length: median is one of the digits by definition. */
    if (len & 1U) return true;

    /* Even length: find the two middle digits in sorted order. */
    size_t lower_pos = len / 2 - 1;
    size_t upper_pos = len / 2;
    int d1 = -1, d2 = -1;
    size_t acc = 0;
    for (int d = 0; d < 10; ++d) {
        if (freq[d] == 0) continue;
        if (acc + freq[d] > lower_pos && d1 < 0) d1 = d;
        if (acc + freq[d] > upper_pos   && d2 < 0) d2 = d;
        acc += freq[d];
        if (d1 >= 0 && d2 >= 0) break;
    }
    if (d1 < 0 || d2 < 0) return false;

    /* Median must be integer and present as a digit. */
    if (((d1 + d2) & 1) != 0) return false;          /* non‑integral median */
    int median = (d1 + d2) >> 1;
    return freq[median] > 0;
}

/* ------------------------------------------------------------ */
/* Memoised cache (static array)                                 */
/* ------------------------------------------------------------ */

#define MAX_CACHE 200000   /* should be far larger than any requested n */

static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static seq_t next_candidate = 0;

static void generate_terms(size_t need)
{
    if (cache_len >= need) return;
    while (cache_len < need) {
        if (has_property(next_candidate)) {
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
        /* safety guard – the sequence grows roughly linearly */
        if (cache_len >= MAX_CACHE) abort();
    }
}

/* ------------------------------------------------------------ */
/* Required public function                                      */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n==0 but safe to return 0 */
    generate_terms((size_t)n);
    return cache[n - 1];
}
