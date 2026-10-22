/*
 * A389718  a(n) = Sum_{k=0..floor(5*n/14)} binomial(k,5*n-14*k).
 * https://oeis.org/A389718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(5*n/14)} binomial(k, 5*n-14*k)
 * Range:     u8 n <= 31, u16 n <= 54, u32 n <= 103, u64 n <= 196,
 *            u128 n <= 366, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

/* multiplicative binomial C(n,k) for idx_t arguments, works for both
 * small-width unsigned types (using 128‑bit intermediates) and for
 * arbitrary‑precision seq_t types.  Values exceeding seq_t width
 * simply overflow the type, which is acceptable for the 64‑bit build
 * but the arbitrary‑precision build will hold the full value. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Compute res = res * (n - k + i) / i */
        idx_t mult = n - k + i;
#if defined(__SIZEOF_INT128__)  /* use 128‑bit intermediate if available */
        if (sizeof(seq_t) <= 8) {
            __uint128_t prod = (__uint128_t)res * mult;
            res = (seq_t)(prod / i);
        } else
#endif
        {
            res = res * (seq_t)mult / (seq_t)i;
        }
    }
    return res;
}

/* External function required by the build */
seq_t a(idx_t n)
{
    /* Upper bound for binomial summation */
    idx_t kmax = (5 * n) / 14;

    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t m = 5 * n - 14 * k;   /* second argument of binomial */
        if (m > k) continue;        /* binomial(k,m) is zero if m>k */
        sum += binom(k, m);
    }
    return sum;
}
