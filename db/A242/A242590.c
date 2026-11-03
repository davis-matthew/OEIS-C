/*
 * A242590  Numbers whose representation in Roman numerals is horizontally
 *          symmetrical.
 * https://oeis.org/A242590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = nth positive integer whose Roman numeral representation
 *            contains only the horizontally symmetrical letters I, X, C, and
 *            D
 * Range:     u8 n <= 69, u16 n <= 224, u32 n <= 224, u64 n <= 224,
 *            u128 n <= 224, big any n
 * Validated: full, pass, 224 terms through n = 224 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Checks whether the Roman numeral of x uses only the allowed letters. */
static bool roman_is_allowed(int x) {
    /* Value/figure pairs, in descending order. */
    static const int values[]     = {1000, 900, 500, 400, 100, 90,
                                     50,   40,   10,   9,   5,   4,   1};
    static const char *symbols[]  = {"M",  "CM", "D",  "CD", "C",  "XC",
                                     "L",  "XL", "X",  "IX", "V",  "IV", "I"};

    for (int i = 0; i < 13; ++i) {
        while (x >= values[i]) {
            const char *p = symbols[i];
            /* All symbols must consist only of I, X, C, or D. */
            while (*p) {
                if (*p != 'I' && *p != 'X' && *p != 'C' && *p != 'D')
                    return false;
                ++p;
            }
            x -= values[i];
        }
    }
    return true;
}

/* Static cache for the sequence (only 224 terms). */
static seq_t seq_cache[224];
static idx_t seq_len = 0;
static bool seq_built = false;

seq_t a(idx_t n) {
    if (!seq_built) {
        /* Build the cache once. */
        seq_len = 0;
        for (int num = 1; num <= 899 && seq_len < 224; ++num) {
            if (roman_is_allowed(num))
                seq_cache[seq_len++] = (seq_t)num;
        }
        seq_built = true;
    }
    if (n == 0 || n > seq_len)
        return 0;                     /* out of bounds; sequence ends at 899 */
    return seq_cache[n - 1];
}
