/*
 * A343800  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(6,n) (with n at least 4) missing three edges, where
 *          exactly two removed edges are incident to the same vertex in the
 *          6-point set and exactly two removed edge...
 * https://oeis.org/A343800
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 465*63^(n-2) - 1110*31^(n-2) + 967*15^(n-2) - 388*7^(n-2)
 *            + 70*3^(n-2) - 4 (n >= 4) The implementation uses the closed
 *            form directly. For implementations where seq_t is larger than 64
 *            bits (or a built‑in big‑int type) the helper function big_pow is
 *            used. For 64‑bit seq_t a small 128‑bit in...
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset (not used by the tests) */
    if (n < 4) return 0;

    idx_t exp = n - 2;          /* exponent for the powers */

    /* Branch for big integer types (size > 64 bits) */
    if (sizeof(seq_t) > sizeof(unsigned long long)) {
        seq_t pow63 = big_pow((seq_t)63, (seq_t)exp);
        seq_t pow31 = big_pow((seq_t)31, (seq_t)exp);
        seq_t pow15 = big_pow((seq_t)15, (seq_t)exp);
        seq_t pow7  = big_pow((seq_t)7,  (seq_t)exp);
        seq_t pow3  = big_pow((seq_t)3,  (seq_t)exp);

        seq_t result = (seq_t)465 * pow63;
        result -= (seq_t)1110 * pow31;
        result += (seq_t)967  * pow15;
        result -= (seq_t)388  * pow7;
        result += (seq_t)70   * pow3;
        result -= (seq_t)4;
        return result;
    }

    /* Small 64‑bit seq_t path – use 128‑bit intermediates. */
    unsigned __int128 p63 = 1, p31 = 1, p15 = 1, p7 = 1, p3 = 1;
    for (idx_t i = 0; i < exp; ++i) {
        p63 *= 63;
        p31 *= 31;
        p15 *= 15;
        p7  *= 7;
        p3  *= 3;
    }

    unsigned __int128 term1 = (unsigned __int128)465 * p63;
    unsigned __int128 term2 = (unsigned __int128)1110 * p31;
    unsigned __int128 term3 = (unsigned __int128)967  * p15;
    unsigned __int128 term4 = (unsigned __int128)388  * p7;
    unsigned __int128 term5 = (unsigned __int128)70   * p3;

    unsigned __int128 res = term1 - term2 + term3 - term4 + term5 - 4;
    return (seq_t)res;
}
