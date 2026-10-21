/*
 * A290323  Minimal number of supporters among total of n voters that may make
 *          (but not guarantee) their candidate win in a multi-level selection
 *          based on the majority vote at each level.
 * https://oeis.org/A290323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) = c(m) * Π (p+1)/2^e, where n = 2^m · Π p^e (p odd primes),
 *            c(0)=1, c(1)=2, c(3b) = 5^b, c(3b+2) = 3·5^b, c(3b+4) = 9·5^b
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

/* compute 5^b in seq_t */
static seq_t pow5(idx_t b)
{
    seq_t v = 1;
    for (idx_t i = 0; i < b; ++i) v *= 5;
    return v;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined below offset 1 */

    /* separate power of 2 */
    uint64_t nn = (uint64_t)n;
    idx_t m = 0;
    while ((nn & 1ULL) == 0ULL) {
        nn >>= 1;
        ++m;
    }

    /* factor odd part */
    seq_t prod = 1;
    if (nn > 1ULL) {
        /* maximum number of distinct prime factors of a 64‑bit integer < 64 */
        uint64_t pr[64];
        uint64_t ex[64];
        size_t cnt = 0;
        factor_u64(nn, pr, ex, &cnt);
        for (size_t i = 0; i < cnt; ++i) {
            /* p is odd */
            seq_t term = (seq_t)(pr[i] + 1) / 2;
            for (uint64_t e = 0; e < ex[i]; ++e) prod *= term;
        }
    }

    /* compute c(m) */
    seq_t c;
    if (m == 0) {
        c = 1;
    } else if (m == 1) {
        c = 2;
    } else {
        idx_t b = m / 3;
        switch (m % 3) {
            case 0:                     /* m = 3b  */
                c = pow5(b);
                break;
            case 1:                     /* m = 3b+4 (m>1) */
                c = 9 * pow5((m - 4) / 3);
                break;
            case 2:                     /* m = 3b+2 */
                c = 3 * pow5((m - 2) / 3);
                break;
            default:
                c = 1;                  /* unreachable */
        }
    }

    return prod * c;
}
