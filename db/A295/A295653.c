/*
 * A295653  Square array T(n, k), n >= 0, k >= 0, read by antidiagonals
 *          upwards: T(n, k) = the (k+1)-th nonnegative number m such that n
 *          AND m = 0 (where AND denotes the bitwise AND operator).
 * https://oeis.org/A295653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is read from the square array T(n,k) by anti‑diagonals.
 * For each n ≥ 0 let
 *   d = ⌊(√(8 n + 1) − 1)/2⌋                // diagonal index
 *   prev = d · (d + 1)/2                     // number of terms before this diagonal
 *   k = n − prev                             // offset in the diagonal
 * Then a(n) = T(d − k, k), where T is defined by
 *   T(0, k) = k,
 *   T(2 m + 1, k) = 2 · T(m, k),
 *   T(2 m, 2 k) = 2 · T(m, k),
 *   T(2 m, 2 k + 1) = 2 · T(m, k) + 1.
 * The function below implements this mapping.
 */
#include <math.h>
#include <stdint.h>

/* compute T(n,k) : the (k+1)‑th non‑negative m with n & m = 0 */
static seq_t T_value(idx_t n, idx_t k)
{
    seq_t result = 0;
    idx_t shift = 0;
    idx_t tmp_n = n;
    idx_t tmp_k = k;

    while (tmp_n != 0 || tmp_k != 0) {
        if ((tmp_n & 1ULL) == 0) {          /* n has a 0 bit here → use a bit of k */
            if (tmp_k & 1ULL) {
                result |= ((seq_t)1 << shift);
            }
            tmp_k >>= 1;
        }
        tmp_n >>= 1;
        ++shift;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 1. find diagonal d  */
    long double val = sqrtl((long double)8 * n + 1);
    idx_t d = (idx_t)((val - 1.0L) / 2.0L);  /* initial approximation */

    /* correct possible rounding errors caused by floating precision */
    while ((((__int128)(d + 1) * (d + 2)) / 2) <= n) ++d;
    while ((((__int128)d * (d + 1)) / 2) > n) --d;

    /* 2. position k inside diagonal d  */
    idx_t prev = (d * (d + 1)) / 2;         /* number of terms before diagonal d */
    idx_t k    = n - prev;
    idx_t row  = d - k;
    idx_t col  = k;

    /* 3. evaluate T(row, col) */
    return T_value(row, col);
}
