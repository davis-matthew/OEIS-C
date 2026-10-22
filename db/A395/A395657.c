/*
 * A395657  Expansion of 1/(16*x) * (1/(5-4*g)^4 - 1), where g = 1+x*g^5 is
 *          the g.f. of A002294.
 * https://oeis.org/A395657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(n+1) * Sum_{k=0..n} 4^k * binomial(k+4,4) *
 *            binomial(5*n+5,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Helper for 128‑bit arithmetic on 64‑bit builds.
   When seq_t is 64‑bit the intermediate sums are accumulated in
   unsigned __int128 to avoid overflows.  For larger seq_t the
   cast is lost and we just use seq_t for the sum. */
#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 int128_t;
#else
typedef seq_t int128_t;
#endif

static inline seq_t mul3(seq_t a, seq_t b, seq_t c)
{
    /* product may overflow seq_t when seq_t is 64‑bit; use 128‑bit for the
       intermediate. */
#if defined(__SIZEOF_INT128__)
    return (seq_t)(((unsigned __int128)a * b) * c);
#else
    return a * b * c;
#endif
}

static inline seq_t muldiv(seq_t a, seq_t b, seq_t c)
{
    /* compute a * b / c with 128‑bit intermediate if needed. */
#if defined(__SIZEOF_INT128__)
    return (seq_t)(((unsigned __int128)a * b) / c);
#else
    return a * b / c;
#endif
}

seq_t a(idx_t n)
{
    idx_t N = 5 * n + 5;
    seq_t pow4   = 1;                     /* 4^k */
    seq_t binom4k = 1;                     /* binomial(k+4,4) */
    seq_t binom_cur = binomial(N, n);      /* binomial(N, n) */
    int128_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* term = 4^k * C(k+4,4) * C(N, n-k) */
        int128_t term = (int128_t)pow4 * (int128_t)binom4k * (int128_t)binom_cur;
        sum += term;

        if (k == n) break;

        /* Update pow4 = 4^(k+1) */
        pow4 = mul3(pow4, (seq_t)4, (seq_t)1);

        /* binom4k = binom(k+5,4) = binom(k+4,4) * (k+5)/(k+1) */
        binom4k = muldiv(binom4k, (seq_t)(k + 5), (seq_t)(k + 1));

        /* Update binom_cur from C(N, n-k) to C(N, n-(k+1)) */
        idx_t m = n - k;                   /* current m = n-k */
        binom_cur = muldiv(binom_cur, (seq_t)m, (seq_t)(N - m + 1));
    }

    /* Divide by (n+1) to obtain a(n) */
    seq_t result = (seq_t)(sum / ((int128_t)n + (int128_t)1));
    return result;
}
