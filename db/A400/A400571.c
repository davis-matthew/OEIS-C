/*
 * A400571  Array A(n,k) read by antidiagonals, where A(n,k) is the surface
 *          area of an n-dimensional hyperrectangle obtained from the unit
 *          n-cube by adding k unit layers, one dimension at a time,
 *          maximizing the surface area at each step.
 * https://oeis.org/A400571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, new
 * Formula:   string a(n) is the surface area A(n,k) of an n‑dimensional *
 *            hyperrectangle after k unit layers, read by antidiagonals. *
 *            A(n,k) is computed from * q = k / n, r = k % n * if r==0: A =
 *            2*n*(q+1)^(n-1) * else: A = 2*(q+1)^(n-r-1)*(q+2)^(r-1)*(n*q +
 *            2*n - r)
 * Range:     u8 n <= 70, u16 n <= 310, u32 n <= 1448, u64 n <= 6364,
 *            u128 n <= 9718, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t idx)
{
    /* Find the antidiagonal s such that s*(s+1)/2 >= idx.
     * Binary search with safe 128‑bit arithmetic. */
    idx_t low = 0, high = 1;
    while (( (__uint128_t)high * (high + 1) ) / 2 < idx) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((((__uint128_t)mid * (mid + 1)) >> 1) >= idx) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t s = low;                     /* s >= 1 */
    idx_t prevTri = (s-1) * s / 2;      /* triangles before this antidiagonal */
    idx_t pos = idx - prevTri;         /* position within diagonal (1..s) */
    idx_t nDim = pos;                  /* dimension n */
    idx_t kLayers = s - nDim;          /* additional layers k */

    /* Compute A(nDim, kLayers). */
    if (nDim == 1) {
        return 2;
    }

    idx_t q = kLayers / nDim;
    idx_t r = kLayers % nDim;

    if (r == 0) {
        seq_t powv = pow_seq((seq_t)(q + 1), (int)(nDim - 1));
        return 2 * (seq_t)nDim * powv;
    } else {
        seq_t pow1 = pow_seq((seq_t)(q + 1), (int)(nDim - r - 1));
        seq_t pow2 = pow_seq((seq_t)(q + 2), (int)(r - 1));
        seq_t factor = (seq_t)nDim * (seq_t)q + (seq_t)(2 * nDim - r);
        return 2 * pow1 * pow2 * factor;
    }
}
