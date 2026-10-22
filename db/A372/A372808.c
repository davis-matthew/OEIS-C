/*
 * A372808  a(n) = sum of the digits (mod 5) of 5^n.
 * https://oeis.org/A372808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{digits d of 5^n} (d mod 5)
 * Range:     u8 n <= 176, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static state to memoise computed values */
static seq_t *a_cache   = NULL;   /* a(n) values */
static size_t a_cap     = 0;      /* allocated capacity */
static size_t a_len     = 0;      /* number of values currently stored */

/* Buffer holding the decimal digits of the current 5^k, least‑significant
   digit first.  The array grows as needed. */
static uint8_t *dig_buf = NULL;
static size_t   d_cap   = 0;
static size_t   d_len   = 0;

/* Ensure that a_cache has at least 'needed' entries. */
static void ensure_a_cache(size_t needed)
{
    if (needed <= a_cap)
        return;
    size_t new_cap = a_cap ? a_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = newp;
    a_cap   = new_cap;
}

/* Ensure that dig_buf can hold at least 'needed' digits. */
static void ensure_dig_buf(size_t needed)
{
    if (needed <= d_cap)
        return;
    size_t new_cap = d_cap ? d_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    uint8_t *newp = (uint8_t *)realloc(dig_buf, new_cap * sizeof(uint8_t));
    dig_buf = newp;
    d_cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* First call: initialise static arrays */
    if (a_cache == NULL) {
        a_cap   = 64;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_len   = 0;

        d_cap   = 64;
        dig_buf = (uint8_t *)malloc(d_cap * sizeof(uint8_t));
        d_len   = 0;
    }

    /* Grow the cache if necessary */
    if ((size_t)n >= a_cap)
        ensure_a_cache((size_t)n + 1);

    /* Compute values up to n that have not yet been cached */
    for (size_t i = a_len; i <= (size_t)n; ++i) {
        if (i == 0) {           /* 5^0 = 1 */
            d_len   = 1;
            dig_buf[0] = 1;
            a_cache[0] = (seq_t)1;
            continue;
        }

        /* Multiply the current decimal number by 5 */
        uint32_t carry = 0;
        for (size_t pos = 0; pos < d_len; ++pos) {
            uint32_t prod = dig_buf[pos] * 5u + carry;
            dig_buf[pos] = (uint8_t)(prod % 10u);
            carry        = prod / 10u;
        }
        while (carry) {
            ensure_dig_buf(d_len + 1);
            dig_buf[d_len++] = (uint8_t)(carry % 10u);
            carry /= 10u;
        }

        /* Sum each digit modulo 5 */
        seq_t sum = 0;
        for (size_t pos = 0; pos < d_len; ++pos)
            sum += (seq_t)(dig_buf[pos] % 5u);

        a_cache[i] = sum;
    }

    a_len = (size_t)n + 1;
    return a_cache[(size_t)n];
}
