/*
 * A348665  Number of partitions of n into 3 parts whose smallest and middle
 *          parts divide n.
 * https://oeis.org/A348665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..floor(n/3)} Sum_{i=j..floor((n-j)/2)} c(n/j) *
 *            c(n/i) * where c(x) = 1 - ceil(x) + floor(x) (equals 1 if x is
 *            integer, 0 otherwise)
 * Range:     u8 n <= 719, u16 n <= 7447, u32 n <= 7447, u64 n <= 7447,
 *            u128 n <= 7447, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 no partition into 3 positive parts exists */
    if (n < 3) return 0;

    /* Gather all divisors of n (up to about 128 for n <= 10000) */
    idx_t divs[256];
    idx_t sz = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            divs[sz++] = d;
            if (d * d != n)
                divs[sz++] = n / d;
        }
    }

    seq_t count = 0;
    for (idx_t jj = 0; jj < sz; ++jj) {
        idx_t j = divs[jj];
        if (j > n / 3) continue;             /* j must be <= floor(n/3) */
        for (idx_t ii = 0; ii < sz; ++ii) {
            idx_t i = divs[ii];
            if (i < j) continue;             /* require i >= j          */
            if (j + 2 * i > n) continue;      /* ensure j + 2*i <= n     */
            ++count;                          /* valid (j,i) pair         */
        }
    }
    return count;
}
