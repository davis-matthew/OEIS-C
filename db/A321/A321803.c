/*
 * A321803  Keep only consecutive identical decimal digits of n; write 0 if
 *          all digits disappear.
 * https://oeis.org/A321803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 199, u16 n <= 199, u32 n <= 199, u64 n <= 199,
 *            u128 n <= 199, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = keep only digits that have a consecutive identical
 * neighbour.  Return 0 if no such digits.
 *   e.g. 123321 → 33, 1123 → 11, 100223 → 22, 12321 → 0.
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* collect digits of n (least‑significant first) */
    unsigned char digs[64];
    size_t len = 0;
    idx_t tmp = n;
    do
    {
        digs[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    } while (tmp);

    /* reverse to most‑significant first */
    for (size_t i = 0; i < len / 2; ++i)
    {
        unsigned char t = digs[i];
        digs[i]     = digs[len - 1 - i];
        digs[len-1-i] = t;
    }

    /* build the sequence of kept digits */
    unsigned char out[64];
    size_t outlen = 0;
    for (size_t i = 0; i < len; )
    {
        size_t j = i;
        while (j + 1 < len && digs[j] == digs[j + 1]) ++j;
        if (j > i)           /* run of length ≥ 2 */
            for (size_t k = i; k <= j; ++k)
                out[outlen++] = digs[k];
        i = j + 1;
    }

    /* strip leading zeros; if none left return 0 */
    size_t pos = 0;
    while (pos < outlen && out[pos] == 0) ++pos;
    if (pos == outlen) return (seq_t)0;

    /* convert remaining digits to a seq_t value */
    seq_t res = 0;
    for (size_t i = pos; i < outlen; ++i)
        res = res * 10 + (seq_t)out[i];

    return res;
}
