/*
 * A356726  Integers which have in Roman numerals more distinct symbols than
 *          any smaller number.
 * https://oeis.org/A356726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A356726 by scanning successive integers and recording
   those that set a new high on the number of distinct Roman symbols.
   The sequence is finite – after 1444 all symbols have been used. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Roman numeral values (int is safe up to 1000). */
static const int roman_vals[13] = {
    1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1
};

/* Bit masks for the Roman symbols:
   I=0, V=1, X=2, L=3, C=4, D=5, M=6.
   Each entry ORs the bits of the symbols used in the subtractive pair. */
static const int roman_masks[13] = {
    1 << 6,                    /* M      */
    (1 << 4) | (1 << 6),       /* CM     */
    1 << 5,                    /* D      */
    (1 << 4) | (1 << 5),       /* CD     */
    1 << 4,                    /* C      */
    (1 << 2) | (1 << 4),       /* XC     */
    1 << 3,                    /* L      */
    (1 << 2) | (1 << 3),       /* XL     */
    1 << 2,                    /* X      */
    (1 << 0) | (1 << 2),       /* IX     */
    1 << 1,                    /* V      */
    (1 << 0) | (1 << 1),       /* IV     */
    1 << 0                     /* I      */
};

/* Helper: population count for 8‑bit values. */
static unsigned char popcount8(unsigned char x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcount((unsigned int)x) & 0xFF;
#else
    unsigned char c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
#endif
}

/* Number of distinct Roman symbols used for a given integer. */
static unsigned char distinct_roman(uint64_t num)
{
    unsigned char mask = 0;
    uint64_t rem = num;
    for (size_t i = 0; i < 13; ++i) {
        if (rem >= (uint64_t)roman_vals[i]) {
            uint64_t times = rem / (uint64_t)roman_vals[i];
            if (times != 0)
                mask |= (unsigned char)roman_masks[i];
            rem -= times * (uint64_t)roman_vals[i];
        }
    }
    return popcount8(mask);
}

/* Return a(n) for A356726. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Static state: list of record numbers, current scan value,
       number of records found, and the highest distinct count seen. */
    static seq_t *records = NULL;
    static size_t rec_len = 0, rec_cap = 0;
    static unsigned char max_distinct = 0;
    static uint64_t scan = 1;          /* next integer to examine */

    /* If we already have enough records, return the stored one. */
    if (rec_len >= (size_t)n)
        return records[n - 1];

    /* Initialise the records array lazily. */
    if (records == NULL) {
        rec_cap = 8;
        records = (seq_t *)malloc(rec_cap * sizeof(seq_t));
        if (records == NULL)
            return 0;   /* allocation failure – return 0 */
    }

    /* Scan integers until we have enough records or all symbols are used. */
    while (rec_len < (size_t)n) {
        unsigned char d = distinct_roman(scan);

        if (d > max_distinct) {
            /* New record: store it. */
            if (rec_len + 1 > rec_cap) {
                rec_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(records, rec_cap * sizeof(seq_t));
                if (tmp == NULL)
                    return 0;   /* allocation failure */
                records = tmp;
            }
            records[rec_len] = (seq_t)scan;
            ++rec_len;
            max_distinct = d;
        }

        /* After all 7 symbols have been seen we cannot find a new record. */
        if (max_distinct == 7 && rec_len < (size_t)n)
            break;

        ++scan;
    }

    /* If we did not reach the requested index, return 0. */
    if (rec_len < (size_t)n)
        return 0;

    return records[n - 1];
}
