/*
 * A248090  Square array read by antidiagonals: T(n,k) is the number of k-edge
 *          colored trees on vertex set [n] (n>=2, k>=2).
 * https://oeis.org/A248090
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n,k) = k*(n-2)!*C((k-1)n, n-2) with the pair (n,k)
 *            obtained by reading the square array by antidiagonals
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute floor(sqrt(x)) for a 128‑bit unsigned value. */
static idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t lo = 0, hi = 1;
    while ((unsigned __int128)hi * hi <= x)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0; /* sequence starts at offset 2 */

    /* Translate the sequence index to the table coordinates (n,k). */
    idx_t t = n - 2;                /* 0‑based position in the enumeration */
    unsigned __int128 v = (unsigned __int128)8 * t + 1;
    idx_t root = isqrt_u128(v);
    idx_t s = (root - 1) >> 1;       /* 0‑based diagonal index */
    idx_t prev = (s * (s + 1)) / 2;  /* number of elements before this diagonal */
    idx_t pos = t - prev;            /* position inside the diagonal */

    idx_t r = pos;
    idx_t c = s - pos;
    idx_t n_v = r + 2;               /* actual n >= 2 */
    idx_t k_v = c + 2;               /* actual k >= 2 */

    /* Compute (n_v-2)! */
    idx_t m = n_v - 2;
    seq_t fact = 1;
    for (idx_t i = 2; i <= m; ++i)
        fact *= (seq_t)i;

    /* Compute C((k_v-1)*n_v, m) */
    idx_t a = (k_v - 1) * n_v;
    idx_t b = m;
    seq_t bin = 1;
    for (idx_t i = 1; i <= b; ++i)
        bin = bin * (seq_t)(a - b + i) / (seq_t)i;

    /* Assemble the final result: k_v * fact * bin */
    seq_t result = fact * (seq_t)k_v * bin;
    return result;
}
