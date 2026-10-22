/*
 * A358015  a(n) = DedekindPsi(n*2^(-k))*2^(j-1) where k = valuation(n, 2) and
 *          j = k if 4 divides n and otherwise 0.
 * https://oeis.org/A358015
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = DedekindPsi(OddPart(n))/2 * [4 divides n ? EvenPart(n) :
 *            1]
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* which simplifies to: if k = valuation(n,2) is 0 or 1,
 *      a(n) = DedekindPsi(n>>k)/2
 *   else (k>=2)
 *      a(n) = DedekindPsi(n>>k) * 2^(k-1)          */
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* undefined for the offset */

    uint64_t nn = (uint64_t)n;          /* n fits in 64‑bit */
    uint64_t k  = __builtin_ctzll(nn);   /* valuation of n at 2 */
    uint64_t m  = nn >> k;              /* odd part */

    /* compute DedekindPsi(m) */
    seq_t dedup = (seq_t)m;
    uint64_t tmp = m;
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;          /* remove all powers of p */
            dedup = dedup / (seq_t)p * ((seq_t)p + 1);
        }
    }
    if (tmp > 1) {                                /* remaining prime factor */
        dedup = dedup / (seq_t)tmp * (((seq_t)tmp) + 1);
    }

    if (k < 2) {                            /* n odd or n = 2*m, m odd */
        return dedup / 2;                   /* guaranteed integer */
    } else {                                /* n divisible by 4 */
        seq_t pow = pow2(k - 1);             /* 2^(k-1) */
        return dedup * pow;
    }
}
