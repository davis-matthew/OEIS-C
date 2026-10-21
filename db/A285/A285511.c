/*
 * A285511  Value of the n-th Roman number interpreted as Latin alphabetic
 *          number.
 * https://oeis.org/A285511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   a(n) = alphabetic value of the Roman numeral of n (interpreted
 *            in base 26 with A = 1, B = 2, …, Z = 26)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 37, u64 n <= 2887,
 *            u128 n <= 3999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

typedef struct {
    idx_t val;          /* integer value of the Roman part   */
    const char *sym;    /* string representation (e.g. "CM")*/
} roman_pair_t;

/* Roman numeral table for conversion.  The values are in descending
   order to allow greedy construction of the numeral. */
static const roman_pair_t _roman[] = {
    {1000, "M"},
    {900,  "CM"},
    {500,  "D"},
    {400,  "CD"},
    {100,  "C"},
    {90,   "XC"},
    {50,   "L"},
    {40,   "XL"},
    {10,   "X"},
    {9,    "IX"},
    {5,    "V"},
    {4,    "IV"},
    {1,    "I"}
};

seq_t a(idx_t n) {
    seq_t res = 0;
    idx_t rem = n;
    const size_t roman_len = sizeof(_roman) / sizeof(_roman[0]);

    for (size_t i = 0; i < roman_len; ++i) {
        while (rem >= _roman[i].val) {
            /* process each letter of the Roman part */
            const char *p = _roman[i].sym;
            while (*p) {
                int v = (unsigned char)(*p) - 'A' + 1;  /* letter value 1‑26 */
                res = res * (seq_t)26 + (seq_t)v;
                ++p;
            }
            rem -= _roman[i].val;
        }
    }
    return res;
}
