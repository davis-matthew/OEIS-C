/*
 * A249890  Number of length 7+6 0..n arrays with no seven consecutive terms
 *          having the maximum of any three terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A249890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the degree‑13 polynomial a(n) = n^13 - (10963/4620)*n^12
 *            + (90131/7700)*n^11 - (265259/15120)*n^10 + (644381/15120)*n^9 -
 *            (442943/10080)*n^8 + (322067/6300)*n^7 + (1061/252)*n^6 -
 *            (62261/1008)*n^5 + (508283/6048)*n^4 - (1313561/37800)*n^3 -
 *            (21229/3080)*n^2 + (8746/1155)*n . The implement...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* first 14 terms of the sequence (a(1) .. a(14)) */
    static const seq_t first[14] = {
        35ULL,
        20942ULL,
        2027106ULL,
        67100334ULL,
        1111583581ULL,
        11450986236ULL,
        83742245992ULL,
        473488497480ULL,
        2192694269979ULL,
        8660886469382ULL,
        30052094475746ULL,
        93649537009470ULL,
        266568651310221ULL,
        702331289801020ULL
    };

    static seq_t delta[14];
    static bool initialized = false;
    if (!initialized) {
        seq_t temp[14];
        for (int i = 0; i < 14; ++i) temp[i] = first[i];
        delta[0] = temp[0];
        for (int k = 1; k < 14; ++k) {
            for (int i = 0; i < 14 - k; ++i) {
                temp[i] = temp[i + 1] - temp[i];
            }
            delta[k] = temp[0];
        }
        initialized = true;
    }

    seq_t res = 0;
    seq_t binom = 1;                 /* C(m,0)  */
    seq_t m = (seq_t)(n - 1);        /* n-1 fits in idx_t */
    for (int k = 0; k < 14; ++k) {
        if (k > (int)m) break;       /* further binomials are zero */
        res += delta[k] * binom;
        if (k == 13) break;          /* highest needed term processed */
        /* Update binom to C(m,k+1) */
        seq_t numer = binom * (m - (seq_t)k);
        binom = numer / ((seq_t)k + 1);
    }
    return res;
}
