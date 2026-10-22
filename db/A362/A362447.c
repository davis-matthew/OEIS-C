/*
 * A362447  Array A(n,k) (n>=0, k>=0) read by antidiagonals: A(n,k) = 1 if the
 *          English names for n and k have a letter in common, otherwise 0.
 * https://oeis.org/A362447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, word
 * Formula:   not stated
 * Range:     u8 n <= 8471, u16 n <= 8471, u32 n <= 8471, u64 n <= 8471,
 *            u128 n <= 8471, big any n
 * Helpers:   _add_word, _integer_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A362447: a(n) = 1 if the English names of the pair of
   indices at antidiagonal position n share at least one common
   letter; otherwise 0.  The pair (i,j) is obtained by mapping the
   linear index n to coordinates on the anti‑diagonal lattice:
   if d is the smallest integer with n < (d+1)(d+2)/2 then
   start = d*(d+1)/2, i = n - start, j = d - i.  */
/* ------------------------------------------------------------------ */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "../utils/_add_word.h"
#include "../utils/_integer_sqrt.h"

static const char *const _units[10] = {
    "zero","one","two","three","four","five","six","seven","eight","nine"
};
static const char *const _teens[10] = {
    "ten","eleven","twelve","thirteen","fourteen","fifteen",
    "sixteen","seventeen","eighteen","nineteen"
};
static const char *const _tens[10] = {
    "", "", "twenty","thirty","forty","fifty",
    "sixty","seventy","eighty","ninety"
};
static const char *const _scales[] = {
    "", "thousand", "million", "billion", "trillion",
    "quadrillion", "quintillion", "sextillion",
    "septillion", "octillion", "nonillion", "decillion",
    "undecillion", "duodecillion", "tredecillion",
    "quattuordecillion", "quindecillion", "sexdecillion",
    "septendecillion", "octodecillion", "novemdecillion",
    "vigintillion"
};

/* Compute the 26‑bit letter mask of the English name of n. */
static inline uint32_t _mask_of_number(idx_t n)
{
    uint32_t mask = 0;

    if (n == 0) {
        _add_word("zero", &mask);
        return mask;
    }

    idx_t rem = n;
    int group = 0;            /* power‑of‑thousand group */

    while (rem > 0) {
        int chunk = rem % 1000;
        if (chunk) {
            uint32_t sub = 0;

            /* hundreds */
            if (chunk >= 100) {
                int h = chunk / 100;
                _add_word(_units[h], &sub);
                _add_word("hundred", &sub);
                if (chunk % 100)
                    _add_word("and", &sub);
            }

            int tail = chunk % 100;
            if (tail >= 20) {
                int t = tail / 10;
                _add_word(_tens[t], &sub);
                if (tail % 10)
                    _add_word(_units[tail % 10], &sub);
            } else if (tail >= 10) {
                _add_word(_teens[tail - 10], &sub);
            } else if (tail) {
                _add_word(_units[tail], &sub);
            }

            /* scale word (thousand, million, …) */
            if (group > 0 && (size_t)group < sizeof(_scales)/sizeof(*_scales))
                _add_word(_scales[group], &sub);

            mask |= sub;
        }

        rem /= 1000;
        ++group;
    }

    return mask;
}

/* ------------------------------------------------------------------ */
/*                         public sequence function                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Find diagonal d and start index of that diagonal. */
    idx_t d = (_integer_sqrt(1 + 8*n) - 1) / 2;
    idx_t start = d * (d + 1) / 2;
    idx_t i = n - start;
    idx_t j = d - i;

    uint32_t mi = _mask_of_number(i);
    uint32_t mj = _mask_of_number(j);

    return (seq_t)((mi & mj) ? 1 : 0);
}
