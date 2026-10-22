/*
 * A368804  Irregular triangle read by rows: T(n,k) is the k-th balanced
 *          binary word of length 2*n, where words within a row are in
 *          lexicographical order (with leading zeros removed and interpreted
 *          as decimal numbers).
 * https://oeis.org/A368804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 560, u64 n <= 9367,
 *            u128 n <= 9367, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A368804: a(n) is the n‑th term of an irregular triangle.
   The triangle consists of all length‑2*r binary words that contain
   r zeros and r ones, listed lexicographically.  For a given index n
   we locate the row r that contains this term, unrank the (n‑sum_{i<r}
   C(2i,i))’th word of length 2r, strip any leading zeros, and interpret
   the remaining digits as a decimal integer. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* locate the row r that contains position n */
    idx_t sum = 0;
    idx_t r = 1;
    idx_t rowCount;
    while (1) {
        rowCount = (idx_t)binom(2 * r, r);
        if (n <= sum + rowCount)
            break;
        sum += rowCount;
        ++r;
    }

    idx_t k = n - sum;            /* 1‑based within the row */
    int   len = 2 * (int)r;       /* total word length, small */
    char  bits[256];              /* enough for r up to 128 */
    int   zerosLeft = (int)r;
    int   onesLeft  = (int)r;
    idx_t rank = k;               /* keep 1‑based rank */

    for (int pos = 0; pos < len; ++pos) {
        int remain = len - pos - 1;
        idx_t count0 = 0;
        if (zerosLeft > 0)
            count0 = (idx_t)binom(remain, zerosLeft - 1);

        if (rank <= count0) {
            bits[pos] = '0';
            --zerosLeft;
        } else {
            bits[pos] = '1';
            rank -= count0;
            --onesLeft;
        }
    }

    /* find first '1' (to strip leading zeros) */
    int firstOne = 0;
    while (firstOne < len && bits[firstOne] == '0')
        ++firstOne;

    /* interpret the remaining bits as a decimal number */
    seq_t value = 0;
    for (int pos = firstOne; pos < len; ++pos)
        value = value * 10 + (bits[pos] - '0');

    return value;
}
