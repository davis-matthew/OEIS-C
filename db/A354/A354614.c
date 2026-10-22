/*
 * A354614  Number of distinct substrings of length A342263(n) that appear at
 *          least twice in the binary expansion of n.
 * https://oeis.org/A354614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct substrings of length A342263(n) that
 *            appear at least twice in the binary expansion of n. (A342263(n)
 *            = length of the longest repeated substring of that binary
 *            expansion.)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Prepare binary representation of n */
    unsigned char bits[64];
    idx_t len;
    if (n == 0) {
        len = 1;
        bits[0] = 0;
    } else {
        /* compute bit length */
        idx_t t = n;
        len = 0;
        while (t) {
            t >>= 1;
            ++len;
        }
        for (idx_t i = 0; i < len; ++i)
            bits[i] = (n >> (len - 1 - i)) & (idx_t)1;
    }

    seq_t result = 1;                     /* default if nothing repeats */

    /* Search longest repeated substring length from len down to 1 */
    for (idx_t l = len; l >= 1; --l) {
        idx_t m = len - l + 1;             /* number of substrings of length l */
        if (m < 2) continue;              /* cannot repeat */

        unsigned long long val = 0;
        for (idx_t k = 0; k < l; ++k)
            val = (val << 1) | (unsigned long long)bits[k];

        unsigned long long subs[64];
        subs[0] = val;
        unsigned long long mask = (l == 64) ? ~0ULL : ((1ULL << l) - 1ULL);

        for (idx_t i = 1; i < m; ++i) {
            val = ((val << 1) & mask) | (unsigned long long)bits[i + l - 1];
            subs[i] = val;
        }

        /* Sort the substring values (simple bubble sort, m <= 64) */
        for (idx_t i = 0; i < m; ++i)
            for (idx_t j = i + 1; j < m; ++j)
                if (subs[i] > subs[j]) {
                    unsigned long long tmp = subs[i];
                    subs[i] = subs[j];
                    subs[j] = tmp;
                }

        /* Count distinct substrings that appear at least twice */
        idx_t distinct = 0;
        for (idx_t i = 0; i < m; ) {
            idx_t j = i + 1;
            while (j < m && subs[j] == subs[i]) ++j;
            if (j - i >= 2) ++distinct;
            i = j;
        }

        if (distinct > 0) {
            result = distinct;
            break;
        }
    }

    return result;
}
