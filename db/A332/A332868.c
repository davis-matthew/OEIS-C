/*
 * A332868  Number of involutions (plus identity) in a fixed Sylow 2-subgroup
 *          of the symmetric group of degree 2n.
 * https://oeis.org/A332868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of A332757(k+1) for all bit positions k set in
 *            binary expansion of n. A332757(1)=2 and for m>=2: A332757(m) =
 *            A332757(m-1)^2 + 2^{2^{m-1}-1}
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

static seq_t A332757[65];
static int precomp_bits = 0;

static void ensure_precomp(idx_t n)
{
    if (n == 0) return;
    int need = 0;
    idx_t tmp = n;
    while (tmp) {
        need++;
        tmp >>= 1;
    }

    if (need <= precomp_bits) return;

    int old = precomp_bits;
    if (old < 1) {               /* initialize the base value */
        A332757[1] = (seq_t)2;
        old = 1;
    }
    for (int m = old + 1; m <= need; ++m) {
        seq_t prev  = A332757[m - 1];
        seq_t prev2 = prev * prev;
        uint64_t shiftexp = ((uint64_t)1 << (m - 1)) - 1;
        seq_t pow2 = big_pow((seq_t)2, (seq_t)shiftexp);
        A332757[m] = prev2 + pow2;
    }
    precomp_bits = need;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_precomp(n);
    seq_t result = (seq_t)1;
    int bit = 0;
    idx_t t = n;
    while (t) {
        if (t & 1) result *= A332757[bit + 1];
        t >>= 1;
        ++bit;
    }
    return result;
}
