/*
 * A390703  a(n) = Sum_{k=0..floor(n/3)} binomial(3*n-3*k+1,n-3*k).
 * https://oeis.org/A390703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(3*n-3*k+1, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The built‑in binomial() may overflow 64‑bit values.  For 64‑bit
    builds we compute the coefficients with 128‑bit intermediates so
    that the final sum fits into seq_t.  For larger seq_t types the
    provided binomial() is sufficient.  */
#include <stdint.h>
#include "../utils/binomial.h"

static inline seq_t binom_safe(idx_t N, idx_t K)
{
    /*  Safe binomial for 64‑bit seq_t, using __int128 intermediates. */
    if (K > N - K) K = N - K;
    if (K == 0) return (seq_t)1;
    unsigned __int128 r = 1;
    for (idx_t i = 1; i <= K; ++i)
    {
        r = r * (N - K + i);
        r /= i;
    }
    return (seq_t)r;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;                      /* floor(n/3) */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t N = 3 * n - 3 * k + 1;          /* 3n‑3k+1 */
        idx_t K = n - 3 * k;                 /* n‑3k      */
        seq_t c = (sizeof(seq_t) <= 8) ? binom_safe(N, K) : binomial(N, K);
        sum += c;
    }
    return sum;
}
