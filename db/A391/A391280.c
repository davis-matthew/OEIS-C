/*
 * A391280  Array read by ascending antidiagonals: T(n,k) is the number of
 *          primes p in the range n*(k-1) < p <= n*k.
 * https://oeis.org/A391280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is obtained by reading the array T(n,k)=π(n·k)–π(n·(k–1))
 *            along ascending antidiagonals (sum of indices constant). For a
 *            given index n, find the diagonal s with n ≤ s·(s–1)/2, determine
 *            the offset r, then coordinates (n_idx,k)=(s–r,r). Finally
 *            a(n)=T(n_idx,k)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* locate the diagonal */
    double dn = (double)n;
    idx_t s = (idx_t)ceil((1.0 + sqrt(1.0 + 8.0 * dn)) / 2.0);

    /* terms before this diagonal */
    idx_t t_prev = (s - 1) * (s - 2) / 2;

    /* offset within the diagonal (1‑based) */
    idx_t r = n - t_prev;

    /* coordinates of the cell */
    idx_t n_idx = s - r;   /* n coordinate */
    idx_t k     = r;       /* k coordinate */

    /* T(n_idx,k) = π(n_idx·k) – π(n_idx·(k–1)) */
    seq_t upper = compute_pi(n_idx * k);
    seq_t lower = compute_pi(n_idx * (k - 1));
    return upper - lower;
}
