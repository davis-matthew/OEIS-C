/*
 * A252927  Number of nX5 nonnegative integer arrays with upper left 0 and
 *          lower right n+5-6 and value increasing by 0 or 1 with every step
 *          right or down.
 * https://oeis.org/A252927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0}^{min(20,n-1)} p_j * C(n-1-j+20,20) * where p =
 *            {1,-2,224,984,5418,-7437,43066,-112135,198529,-285030, *
 *            339839,-332484,266120,-174781,93817,-40498,13679,-3477,627,-72,4}
 *            * (coefficient of x^j in 1-2x+224x²+…+4x²⁰)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 43, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const long long P[21] = {
    1LL, -2LL, 224LL, 984LL, 5418LL, -7437LL,
    43066LL, -112135LL, 198529LL, -285030LL,
    339839LL, -332484LL, 266120LL, -174781LL,
    93817LL, -40498LL, 13679LL, -3477LL,
    627LL, -72LL, 4LL
};

/* Compute the binomial coefficient C(n,k) for k<=20 */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;   /* division is exact */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1, but safeguard */
    /* Use signed 128-bit to accumulate to avoid overflow during cancellations */
    __int128 acc = 0;
    idx_t maxj = 20;
    if (n - 1 < maxj) maxj = n - 1;
    for (idx_t j = 0; j <= maxj; ++j) {
        idx_t m = n - 1 - j;                 /* m = n-1-j */
        seq_t bin = binomial(m + 20, 20);    /* C(m+20,20) */
        long long coeff = P[j];
        acc += (__int128)bin * coeff;        /* signed accumulation */
    }
    return (seq_t)acc;      /* final result is non‑negative */
}
