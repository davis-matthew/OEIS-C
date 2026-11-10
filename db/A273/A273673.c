/*
 * A273673  Square array A(n,k) = (n / prime(1+A084558(k))^e) *
 *          prime(1+A084558(k)-A099563(k))^e, where e =
 *          A249344((1+A084558(k)), n) = the exponent of the largest power of
 *          prime(1+A084558(k)) which divides n. Array is read by descending
 *          antidiagonals as A(1,1), A(1,2), A(2,1), A(1,3), A(2,2), A(3,1),
 *          etc.
 * https://oeis.org/A273673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   A(n,k) = (n / p^e) * q^e, p = prime(1+A084558(k)), q =
 *            prime(1+A084558(k)-A099563(k)), e = exponent of p in n; a(idx)
 *            reads A by descending antidiagonals
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 136 terms through n = 4844 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static const unsigned A273673_pr[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};

seq_t a(idx_t n)
{
    /* find antidiagonal d with d(d-1)/2 < n <= d(d+1)/2 */
    uint64_t N = (uint64_t)n;
    uint64_t d = (uint64_t)((sqrt(8.0 * (double)N + 1.0) - 1.0) / 2.0);
    if (d < 1) d = 1;
    while (d > 1 && d * (d - 1) / 2 >= N) d--;
    while (d * (d + 1) / 2 < N) d++;
    uint64_t i = N - d * (d - 1) / 2;   /* row, 1..d */
    uint64_t k = d + 1 - i;             /* column */

    /* m = A084558(k), r = A099563(k) */
    unsigned __int128 f = 1;
    unsigned m = 1;
    while (f <= (unsigned __int128)k) {
        m++;
        f *= m;
    }
    m--;
    unsigned __int128 prod = 1;
    for (unsigned t = 2; t <= m; t++) prod *= t;   /* 2*3*...*m */
    uint64_t r = (uint64_t)((unsigned __int128)k / prod);

    unsigned p = A273673_pr[m];          /* prime(1+m) */
    unsigned q = A273673_pr[m - r];      /* prime(1+m-r) */

    seq_t v = (seq_t)i;
    unsigned e = 0;
    while (v % p == 0) {
        v /= p;
        e++;
    }
    while (e--) v *= q;
    return v;
}
