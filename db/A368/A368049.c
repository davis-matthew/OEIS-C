/*
 * A368049  Perfect squares whose decimal expansion consists of k > 1 digits,
 *          k-1 of which are equal.
 * https://oeis.org/A368049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 100*a(n-3) for n > 22. The first 20 terms (≤ 90 000) are
 *            found by brute force; afterwards the terms are of the form
 *            p·10^(2*j) with p∈{1,4,9} and j≥3
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 26, u64 n <= 41, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 936 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The util pow10 calculates 10^e as a seq_t value. */
#include <stdint.h>
#include <stdbool.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Test whether an integer n has exactly two distinct decimal digits,
   one of which occurs once. */
static bool is_good_square(uint32_t n)
{
    if (n < 10)                /* at least two digits required */
        return false;
    int counts[10] = {0};
    int len = 0;
    uint32_t m = n;
    while (m > 0) {
        int d = m % 10;
        counts[d]++;
        m /= 10;
        len++;
    }

    int c1 = -1, c2 = -1;
    for (int d = 0; d < 10; ++d) {
        if (counts[d] > 0) {
            if (c1 == -1)
                c1 = counts[d];
            else if (c2 == -1)
                c2 = counts[d];
            else
                return false;      /* more than two distinct digits */
        }
    }
    if (c2 == -1)  /* only one distinct digit */
        return false;

    return ((c1 == 1 && c2 == len - 1) ||
            (c2 == 1 && c1 == len - 1));
}
#include <stdbool.h>
#include <stdint.h>

/* Temporary storage for the small terms (≤ 90 000).  */
static seq_t   small[300];
static idx_t   small_cnt = 0;
static bool    small_inited = false;

/* Initialise the table of small terms.  */
static void init_small(void)
{
    if (small_inited)
        return;
    small_inited = true;

    /* 300^2 = 90 000 is the largest square we need to test. */
    for (idx_t i = 2; i <= 300; ++i) {
        uint32_t sq = (uint32_t)i * (uint32_t)i; /* exact square */
        if (is_good_square(sq))
            small[small_cnt++] = (seq_t)sq;
    }
}

seq_t a(idx_t n)
{
    init_small();

    if (n <= small_cnt)          /* early terms are pre‑computed */
        return small[n - 1];

    /* For n > small_cnt the terms follow the pattern:
       a(n) = p·10^(2*j)  (j ≥ 3, p∈{1,4,9}),  with n–small_cnt
       alternating every 3 terms. */
    idx_t offset = n - small_cnt - 1;
    idx_t j      = 3 + offset / 3;   /* base exponent in powers of 10 */
    idx_t pos    = offset % 3;       /* 0 → 1, 1 → 4, 2 → 9 */

    seq_t p = (seq_t)(pos == 0 ? 1 : pos == 1 ? 4 : 9);
    return p * pow10(2 * j);
}
