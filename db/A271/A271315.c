/*
 * A271315  Array T(n,k) read by diagonals: T(n,k) = T(n,k-1) + T(n,k-2) where
 *          T(n,0) = F(n+1), T(n,1) = F(n); F(n) = Fibonacci(n) = A000045(n).
 * https://oeis.org/A271315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the array T(i,j) by antidiagonals.
 *            For every row i, T(i,0) = F(i+1), T(i,1) = F(i) (F is Fibonacci,
 *            F(0)=0,F(1)=1). For j ≥ 2, T(i,j) = T(i,j‑1) + T(i,j‑2). The
 *            sequence a(n) is the enumeration of all T(i,j) with i+j running
 *            from 0 up
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t fibs[2048];
static bool fibs_init = false;

static void init_fibs(void)
{
    if (fibs_init) return;
    fibs[0] = 0;
    fibs[1] = 1;
    for (idx_t k = 2; k < 2048; ++k)
        fibs[k] = fibs[k-1] + fibs[k-2];
    fibs_init = true;
}

seq_t a(idx_t n)
{
    init_fibs();

    /* find diagonal d such that d(d+1)/2 ≤ n < (d+1)(d+2)/2 */
    idx_t d = 0;
    while ((((__uint128_t)(d + 1) * (d + 2)) / 2) <= n)
        ++d;

    /* number of terms before this diagonal */
    idx_t before = ((__uint128_t)d * (d + 1)) / 2;
    idx_t offset = n - before;          /* position on diagonal (0‑based) */
    idx_t i = offset;                   /* row index   */
    idx_t j = d - offset;               /* column index */

    if (j == 0) return fibs[i + 1];
    if (j == 1) return fibs[i];

    seq_t a0 = fibs[i + 1], a1 = fibs[i];
    for (idx_t k = 2; k <= j; ++k) {
        seq_t next = a0 + a1;
        a0 = a1;
        a1 = next;
    }
    return a1;
}
