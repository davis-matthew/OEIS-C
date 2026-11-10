/*
 * A398687  Triangle T(n,k) read by rows that has the A001710(n)-th
 *          permutation in reverse colex order of [1,...,n] as row n.
 * https://oeis.org/A398687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   row n (n>=3) = reverse of the word w, where w is the (n!/2)-th
 *            (0-based) permutation of [1..n] in lexicographic order; n=1:
 *            [1], n=2: [1,2]. Closed-form digits: n even w=(n/2+1, rest
 *            ascending); n odd w=((n+1)/2, (n+3)/2, rest ascending). a(N) =
 *            k-th entry of row n, n(n-1)/2 < N <= n(n+1)/2
 * Range:     u8 n <= 210, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Validated: standard, pass, 107 terms through n = 201 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t N) {
    idx_t n = 1;
    idx_t start = 0; /* number of terms before row n */
    while (start + n < N) {
        start += n;
        n++;
    }
    idx_t k = N - start; /* 1-based position in row */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)k;
    idx_t j = n - k + 1; /* index into reversed word w, 1-based */
    idx_t val;
    if ((n & 1) == 0) {
        idx_t v = n / 2 + 1;
        if (j == 1) {
            val = v;
        } else {
            idx_t t = j - 1;
            val = (t < v) ? t : t + 1;
        }
    } else {
        idx_t v1 = (n + 1) / 2;
        idx_t v2 = v1 + 1;
        if (j == 1) {
            val = v1;
        } else if (j == 2) {
            val = v2;
        } else {
            idx_t t = j - 2;
            val = t;
            if (val >= v1) val++;
            if (val >= v2) val++;
        }
    }
    return (seq_t)val;
}
