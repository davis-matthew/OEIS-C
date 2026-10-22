/*
 * A375602  Triangle read by rows, where row n is a block of length n*(n^2 +
 *          1)/2, which is a permutation of the numbers of its constituents;
 *          see Comments.
 * https://oeis.org/A375602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = P(n) + (L(n)-1)*L*(L(n)^2 - L(n) + 2)/8 where L(n) is the
 *            block number containing n, Ld(n) and Rd(n) are the diagonal
 *            index and offset of n in the Cantor-like numbering. The function
 *            implements the closed‑form computation from the OEIS definition
 * Range:     u8 n <= 235, u16 n <= 9316, u32 n <= 9316, u64 n <= 9316,
 *            u128 n <= 9316, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for unsigned 64‑bit n */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for index 0, but return 0 just in case */

    /* --- Diagonal index Ld and offset Rd -------------------------------- */
    idx_t t  = isqrt_u64(8 * n + 1);          /* floor sqrt(8n+1)   */
    idx_t d  = (t - 1) / 2;                   /* floor((sqrt-1)/2)  */
    while (d * (d + 1) / 2 < n)
        ++d;
    idx_t Ld = d;
    idx_t Rd = n - (Ld - 1) * Ld / 2;

    /* --- Block number L ------------------------------------------------- */
    idx_t cum  = 0;          /* cumulative counts up to previous block   */
    idx_t len  = 0;          /* length of current block                  */
    idx_t L    = 0;          /* block number that contains n            */
    for (L = 1; ; ++L) {
        len = L * (L * L + 1) / 2;   /* length of block L */
        cum += len;
        if (n <= cum)
            break;
    }
    idx_t prevCum = cum - len;    /* count of elements before block L */

    /* --- P(n) computation --------------------------------------------- */
    seq_t P = (seq_t)L * Rd + Ld - (seq_t)L * (L + 1) / 2;

    /* Overrun part in P */
    long long over = (long long)Rd - ((long long)L * L - L + 2) / 2;
    if (over < 0) over = 0;
    P -= (seq_t)over * (over + 1) / 2;

    /* --- Cumulative offset of previous blocks -------------------------- */
    seq_t cumPrev = (seq_t)(L - 1) * L * ((seq_t)L * L - L + 2) / 8;

    return P + cumPrev;
}
