/*
 * A279035  Left-concatenate zeros to 2^(n-1) such that it has n digits. In
 *          the regular array formed by listing the found powers, a(n) is the
 *          sum of (nonzero) digits in column n.
 * https://oeis.org/A279035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by summing the non‑zero digits in column n of the array formed by
   left‑padded powers of two.  The routine precomputes powers of two in decimal
   (base 10) as needed and uses them to evaluate the required digit positions
   efficiently. */

#include <stdint.h>
#include <stdlib.h>

#ifndef ROW_CAP
#define ROW_CAP 20000   /* should cover all rows needed for n up to 10000 */
#endif

static uint8_t **row_digits = NULL;   /* array of pointers to digit arrays (LS‑first) */
static size_t *row_lengths = NULL;    /* decimal digit count of each power */
static idx_t total_rows = 0;          /* number of powers stored (1‑based) */
static int initialized = 0;

/* Initialise the storage for the first power 2^0 = 1 */
static void init_rows(void) {
    row_digits  = (uint8_t**)malloc(sizeof(uint8_t*) * ROW_CAP);
    row_lengths = (size_t*)  malloc(sizeof(size_t) * ROW_CAP);

    row_digits[0] = (uint8_t*)malloc(1);
    row_digits[0][0] = 1;      /* 2^0 = 1 */
    row_lengths[0] = 1;
    total_rows = 1;
}

/* Ensure that powers of two up to row index 'up_to' are available. */
static void ensure_rows(idx_t up_to) {
    if (!initialized) { init_rows(); initialized = 1; }

    while (total_rows < up_to) {
        /* Multiply the previous power by 2 to obtain the next power. */
        const uint8_t *prev   = row_digits[total_rows-1];
        size_t       prev_len = row_lengths[total_rows-1];

        /* The next power can have at most one more decimal digit. */
        uint8_t *next = (uint8_t*)malloc(prev_len + 1);
        int carry = 0;
        size_t i;
        for (i = 0; i < prev_len; ++i) {
            int val = prev[i] * 2 + carry;
            next[i] = val % 10;
            carry   = val / 10;
        }
        if (carry) {
            next[i] = carry;
            ++i;
        }

        row_digits[total_rows]  = next;
        row_lengths[total_rows] = i;
        ++total_rows;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* out of range; offset starts at 1 */

    /* Make sure the power for row n exists. */
    ensure_rows(n);

    /* Find the largest row 'm_max' that can contribute to column n. */
    idx_t m = n;
    while (1) {
        ensure_rows(m);          /* it may be required for the check */
        size_t d = row_lengths[m-1];
        if (m - d > n-1) break;  /* start column > n => no further rows */
        ++m;
    }
    idx_t m_max = m - 1;

    /* Sum the required digits. */
    seq_t sum = 0;
    for (idx_t mm = n; mm <= m_max; ++mm) {
        size_t d = row_lengths[mm-1];
        size_t padZeros = mm - d;
        size_t offset   = n - padZeros;          /* 1‑based offset within the number */
        size_t index    = d - offset;            /* LS‑first index */
        uint8_t digit   = row_digits[mm-1][index];
        if (digit != 0) sum += digit;
    }
    return sum;
}
