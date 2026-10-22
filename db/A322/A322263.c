/*
 * A322263  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = numerator of Sum_{d|n} 1/d^k.
 * https://oeis.org/A322263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   a(n) = numerator of σ_k(n) / n^k, where (n,k) is the entry read
 *            by antidiagonals of the square array A(n,k) = numerator of
 *            Sum_{d|n} 1/d^k
 * Range:     u8 n <= 31, u16 n <= 69, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   seq_gcd, seq_pow, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/sigma_k.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_gcd.h"

extern seq_t a(idx_t n) {
    /* Convert 1‑based index n to diagonal sum d,
       row (n) and column (k). */
    idx_t idx = n - 1;                 /* 0‑based index */
    /* Approximate diagonal using quadratic formula. */
    unsigned long long d = (unsigned long long)((sqrt((double)idx * 8.0 + 1.0) - 1.0) / 2.0);
    /* Correct possible off‑by‑one errors */
    while ( ((unsigned __int128)d * (d + 1)) / 2 <= idx ) d++;

    /* Compute position inside the diagonal */
    unsigned long long t_prev = ((unsigned __int128)(d - 1) * d) / 2;
    unsigned long long pos = idx - t_prev;          /* 0‑based position */
    idx_t row = (idx_t)(pos + 1);                   /* n in the array */
    idx_t k   = (idx_t)(d - row);                   /* k in the array */

    /* Compute σ_k(row) and row^k */
    seq_t sigma = sigma_k(row, (uint8_t)k);
    seq_t denom = seq_pow((seq_t)row, k);

    /* Reduce and return numerator */
    seq_t g = seq_gcd(sigma, denom);
    return sigma / g;
}
