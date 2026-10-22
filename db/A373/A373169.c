/*
 * A373169  Square array read by ascending antidiagonals: T(n,k) =
 *          noz(T(n,k-1) + (k-1)*(n-2) + 1), with T(n,1) = 1, n >= 2, k >= 1,
 *          where noz(n) = A004719(n).
 * https://oeis.org/A373169
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, base, easy
 * Formula:   a(n) = T(n,k) read by ascending antidiagonals, where * T(n,1)=1
 *            and T(n,k)=noz(T(n,k-1)+(k-1)*(n-2)+1) (n>=2)
 * Range:     u8 n <= 131, u16 n <= 9678, u32 n <= 9848, u64 n <= 9848,
 *            u128 n <= 9848, big any n
 * Helpers:   noz
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/noz.h"

/* Helper: integer square root (floor) for 64‑bit values. */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t low = 0, high = (idx_t)1 << 32; /* sqrt(2^64-1) <= 2^32-1 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high; /* floor sqrt */
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* n >= 2 is guaranteed by the OEIS offset. */
    idx_t i0 = n - 2;                 /* zero‑based index within the sequence */
    /* Find minimal t with t(t+1)/2 > i0. */
    const idx_t disc = 1 + 8 * i0;    /* fits in 64 bits for the intended n */
    const idx_t sqrt_d = isqrt_u64(disc);
    idx_t t = (sqrt_d - 1) >> 1;      /* floor((-1+sqrt)/2) */
    if (t * (t + 1) / 2 <= i0)        /* correct for any edge cases */
        ++t;
    /* t >= 1 for i0 >= 0. */
    const idx_t s = t + 2;            /* diagonal sum n + k */
    const idx_t prev = t * (t - 1) / 2;       /* total terms before this diag */
    const idx_t offset = i0 - prev;   /* position within the diagonal, 0‑based */
    const idx_t n_val = (s - 1) - offset;    /* current row index */
    const idx_t k_val = offset + 1;            /* current column index */

    /* Compute T(n_val, k_val). */
    seq_t val = 1;                     /* T(n,1) */
    for (idx_t kk = 2; kk <= k_val; ++kk) {
        seq_t add = (seq_t)(kk - 1) * (seq_t)(n_val - 2) + 1;
        val = val + add;
        val = noz(val);
    }
    return val;
}
