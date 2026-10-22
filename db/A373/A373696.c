/*
 * A373696  a(n) is the least m >= 0 with the same number of digits as n such
 *          that for some permutation p of 0..9, applying p to the digits of n
 *          yields the digits of m.
 * https://oeis.org/A373696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest m ≥ 0 having the same number of digits as n
 *            such that there exists a permutation p of 0..9 with p applied to
 *            each digit of n producing the digits of m. Implementation: map
 *            each distinct digit of n, in order of appearance, to the
 *            smallest unused digit (the first digit may not m...
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Store the digits of n in reverse order (least significant first). */
    unsigned char rev[20];      /* 20 digits are enough for 64‑bit values. */
    int d = 0;                  /* number of digits                           */
    idx_t tmp = n;
    while (tmp > 0) {
        rev[d++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    /* Mapping from original digits to new digits.  0xFF indicates unmapped. */
    unsigned char map[10];
    bool used[10];
    for (int i = 0; i < 10; ++i) {
        map[i] = 0xFF;
        used[i] = false;
    }

    seq_t result = 0;

    /* Process digits from most significant to least. */
    for (int pos = d - 1; pos >= 0; --pos) {
        unsigned char orig = rev[pos];
        if (map[orig] == 0xFF) {
            unsigned char assign;
            if (pos == d - 1) {          /* leading digit: avoid 0          */
                assign = 1;
                while (used[assign])
                    ++assign;
            } else {                     /* any other position              */
                assign = 0;
                while (used[assign])
                    ++assign;
            }
            map[orig]  = assign;
            used[assign] = true;
        }
        result = result * 10 + (seq_t)map[orig];
    }

    return result;
}
