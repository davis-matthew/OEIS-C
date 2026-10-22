/*
 * A396457  Triangle T(n, k) of convolved numbers of k-sections of the
 *          Fibonacci sequence at the first convolution order, for n >= 1 and
 *          1 <= k <= n, read by rows.
 * https://oeis.org/A396457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 39, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A396457: Triangle T(n,k) of convolved k‑sections of the Fibonacci sequence
 * at the first convolution order.  The sequence is read row‑wise from the
 * triangle, i.e.
 * a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), … .
 * Here T(r,k) = A(r-k+1,k,1).
 *
 * A(n,k,1) is defined by
 *   A(n,k,1) = Σ_{j=0}^{⌊(n-1)/2⌋} (-1)^{j·(k-1)} · (n-j) ·
 *              binomial(n-1-j,j) · L(k)^{\,n-1-2j},
 *   where L(k) is the (k‑th) Lucas number (L(0)=2, L(1)=1).
 *
 * We compute a(n) by determining the triangle row r that contains index n,
 * its column k, then evaluating A(r-k+1,k,1) with the above sum.  All
 * arithmetic uses the provided helpers to stay within the limits of seq_t.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

static seq_t LU[512];            /* Lucas numbers */
static int LU_initialized = 0;

/* Initialize Lucas numbers up to 511 (far beyond the limits needed). */
static void init_lucas(void) {
    if (LU_initialized) return;
    LU[0] = (seq_t)2;
    LU[1] = (seq_t)1;
    for (int i = 2; i < 512; ++i) LU[i] = LU[i-1] + LU[i-2];
    LU_initialized = 1;
}

/* Compute A(m,k) for m>=1, k>=1 using the summation formula. */
static seq_t A_mk(idx_t m, idx_t k) {
    init_lucas();
    if (m == 1) return (seq_t)1;
    if (m == 2) return (seq_t)(2 * LU[k]);     /* 2 · L(k) */

    /* Sum over j. Use signed 128‑bit to avoid overflow on 64‑bit builds. */
    __int128 sum = 0;
    idx_t max_j = (m - 1) / 2;
    seq_t Lk = LU[k];

    for (idx_t j = 0; j <= max_j; ++j) {
        seq_t powL = big_pow(Lk, (m - 1 - 2 * j));
        seq_t bin  = binomial(m - 1 - j, j);
        __int128 term = (__int128)(m - j);
        term *= ( __int128)bin;
        term *= ( __int128)powL;

        /* sign = (-1)^{j·(k-1)}.
         * If k is odd, k-1 even → sign = +1 for all j.
         * If k is even, sign depends on j's parity. */
        if ((k % 2 == 0) && (j % 2 == 1)) term = -term;

        sum += term;
    }
    return (seq_t)sum;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Find the triangle row r such that r*(r+1)/2 >= n
     * and (r-1)*r/2 < n.  We use a floating point approximation
     * for r, which is safe for n <= 1e12 and perfectly fine for
     * the limits of the tasks. */
    idx_t r = (idx_t)((sqrt(8.0L * n + 1.0L) - 1.0L) / 2.0L); /* floor */
    if (r * (r + 1) / 2 < n) ++r;

    idx_t k = n - r * (r - 1) / 2;          /* column within the row */
    idx_t m = r - k + 1;                    /* argument of A(m,k) */

    return A_mk(m, k);
}
