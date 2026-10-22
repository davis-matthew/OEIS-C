/*
 * A379658  The balanced ternary expansion of a(n) is obtained by removing a
 *          digit 0 before each nonzero digit in the nonadjacent form of n.
 * https://oeis.org/A379658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by converting n to its non‑adjacent * binary
 *            form (NAF), deleting every 0 that immediately precedes a *
 *            non‑zero digit, and interpreting the remaining digits as
 *            balanced * ternary
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* 1. Build NAF digits (LSB → MSB). */
    int8_t naf[70];        /* enough for 64‑bit n with possible carry */
    size_t len = 0;
    idx_t x = n;
    while (x > 0) {
        if (x & 1) {
            int u = x % 4;                  /* u is 1 or 3 */
            int d = (u == 1) ? 1 : -1;       /* choose sign to make x even */
            naf[len++] = (int8_t)d;
            x -= d;                         /* if d==-1, x+=1 else x-=1 */
        } else {
            naf[len++] = 0;
        }
        x >>= 1;
    }

    /* 2. Remove zeros that precede a non‑zero digit (MSB → LSB). */
    int8_t comp[70];
    size_t clen = 0;
    for (size_t i = len; i-- > 0;) {   /* i runs from len-1 down to 0 */
        int d = naf[i];
        if (d == 0 && i > 0 && naf[i - 1] != 0)
            continue;                  /* skip this zero */
        comp[clen++] = (int8_t)d;
    }

    /* 3. Interpret the remaining digits as balanced ternary. */
    seq_t val = 0;
    for (size_t k = 0; k < clen; ++k) {
        val *= 3;
        if (comp[k] == 1)
            ++val;
        else if (comp[k] == -1)
            --val;
        /* zero leaves val unchanged */
    }

    return val;
}
