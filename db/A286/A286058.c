/*
 * A286058  Positions of 1 in A285666; complement of A285667.
 * https://oeis.org/A286058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A285666, the fixed point of
 *            00->001, 1->010 starting from 00 (computed by iterating the
 *            substitution and scanning)
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9974 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static unsigned char *s = NULL;
    static idx_t len = 0;
    static idx_t ones = 0;

    if (s == NULL) {
        s = (unsigned char *)malloc(2);
        s[0] = 0; s[1] = 0;
        len = 2;
        ones = 0;
    }
    while (ones < n + 2) {
        unsigned char *t = (unsigned char *)malloc((size_t)(len * 3 + 10));
        idx_t m = 0, i = 0, c = 0;
        while (i < len) {
            if (s[i] == 1) {
                t[m++] = 0; t[m++] = 1; t[m++] = 0;
                c += 1;
                i++;
            } else if (i + 1 < len && s[i + 1] == 0) {
                t[m++] = 0; t[m++] = 0; t[m++] = 1;
                c += 1;
                i += 2;
            } else {
                t[m++] = 0;
                i++;
            }
        }
        free(s);
        s = t;
        len = m;
        ones = c;
    }
    {
        idx_t i, cnt = 0;
        for (i = 0; i < len; i++) {
            if (s[i] == 1) {
                cnt++;
                if (cnt == n) return (seq_t)(i + 1);
            }
        }
    }
    return (seq_t)0;
}
