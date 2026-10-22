/*
 * A323462  Smallest number that can be obtained from the "Choix de
 *          Bruxelles", version 2 (A323460) operation applied to n.
 * https://oeis.org/A323462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   the minimal value obtainable from one application of the Choix *
 *            de Bruxelles (version 2) operation, as defined in OEIS A323462.
 *            * For a decimal expansion d1…dk of n: * (i) If a substring
 *            starts with 1 and ends with an even digit, * choose the leftmost
 *            1 and the rightmost even digit; replace that...
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Work with the original value for return if unchanged */
    const idx_t orig = n;

    /* Convert n to its decimal digits (most‑significant first) */
    uint8_t digit[64];
    int k = 0;
    if (n == 0) {
        digit[0] = 0;
        k = 1;
    } else {
        idx_t tmp = n;
        while (tmp != 0) {
            digit[k++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
        /* reverse to most‑significant‑first order */
        for (int i = 0, j = k - 1; i < j; ++i, --j) {
            uint8_t t = digit[i];
            digit[i] = digit[j];
            digit[j] = t;
        }
    }

    /* Find leftmost '1' and rightmost even digit */
    int left1 = -1, rightE = -1;
    for (int i = 0; i < k; ++i) {
        if (digit[i] == 1 && left1 == -1) {
            left1 = i;
        }
    }
    for (int i = k - 1; i >= 0; --i) {
        if ((digit[i] & 1) == 0) {
            rightE = i;
            break;
        }
    }

    /* Case (i): substring replacement */
    if (left1 != -1 && rightE != -1 && left1 < rightE) {
        seq_t sub = 0;
        for (int i = left1; i <= rightE; ++i) {
            sub = sub * 10 + (seq_t)digit[i];
        }
        seq_t half = sub / 2;

        /* Build new number: prefix + half + suffix */
        seq_t res = 0;
        for (int i = 0; i < left1; ++i)
            res = res * 10 + (seq_t)digit[i];

        /* Append digits of half */
        /* Convert half to decimal digits */
        seq_t tmpHalf = half;
        if (tmpHalf == 0) {
            res = res * 10 + (seq_t)0;
        } else {
            /* Temporarily store digits of half */
            uint8_t halfDigits[64];
            int hlen = 0;
            while (tmpHalf != 0) {
                halfDigits[hlen++] = (uint8_t)(tmpHalf % 10);
                tmpHalf /= 10;
            }
            for (int i = hlen - 1; i >= 0; --i)
                res = res * 10 + (seq_t)halfDigits[i];
        }

        for (int i = rightE + 1; i < k; ++i)
            res = res * 10 + (seq_t)digit[i];

        return res;
    }

    /* Case (ii) and (iii): no suitable substring */
    /* Result obtained by halving each even digit simultaneously */
    seq_t hAll = 0;
    for (int i = 0; i < k; ++i) {
        uint8_t d = digit[i];
        if ((d & 1) == 0) d >>= 1;   /* halve even digit */
        hAll = hAll * 10 + (seq_t)d;
    }

    /* If the whole number is even, also consider integer half of n */
    seq_t best = hAll;
    if ((n & 1) == 0) {
        seq_t halfAll = (seq_t)n / 2;
        if (halfAll < best) best = halfAll;
    }

    return best;
}
