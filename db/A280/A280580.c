/*
 * A280580  Triangle read by rows: T(n,k) =
 *          binomial(2*n,2*k)*binomial(2*n-2*k,n-k)/(n+1-k) for 0<=k<=n.
 * https://oeis.org/A280580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k) where m = floor((sqrt(8n+1)-1)/2), k = n -
 *            m(m+1)/2, and T(m,k) = binom(2m,2k) * Catalan(m-k)
 * Range:     u8 n <= 11, u16 n <= 37, u32 n <= 94, u64 n <= 305,
 *            u128 n <= 1045, big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangle read by rows: T(m,k) = binom(2m,2k)*binom(2m-2k,m-k)/(m+1-k) */

/* Utility headers – binom and catalan are provided by the build. */
#include "../utils/binom.h"
#include "../utils/catalan.h"

/* Find the row index m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2. */
static idx_t find_row(idx_t n)
{
    /* Exponential search to get an upper bound */
    idx_t lo = 0, hi = 1;
    while (((__int128)hi * (hi + 1)) / 2 <= n)
        hi <<= 1;  /* double hi until it exceeds the target */

    /* Binary search in [lo,hi] */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (((__int128)mid * (mid + 1)) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t m = find_row(n);              /* row index */
    idx_t before = m * (m + 1) / 2;      /* elements before row m */
    idx_t k = n - before;               /* column index within row m */

    seq_t bin = binom(2 * m, 2 * k);     /* binomial(2m,2k) */
    seq_t cat = catalan(m - k);          /* Catalan(m-k) */

    return bin * cat;                    /* T(m,k) */
}
