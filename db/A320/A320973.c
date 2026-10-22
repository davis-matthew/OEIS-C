/*
 * A320973  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = n^k * Product_{p|n, p prime} (1 + 1/p^k).
 * https://oeis.org/A320973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Flattened antidiagonal of the square array A(n,k)
 * where A(n,k) = n^k * ∏_{p|n} (1 + 1/p^k)
 *
 * For 1‑based index n: find diagonal d with d(d+1)/2 ≥ n,
 * let offset = n - d(d-1)/2 - 1 (0‑based position inside the diagonal),
 * then row i = offset+1, column j = d - i.
 * A(i,j) is evaluated multiplicatively:
 *   For each distinct prime factor p^e of i,
 *       contribution = p^(j*(e-1)) * (p^j + 1)
 */

#include <stdint.h>

/* Small integer power with seq_t as the result type
   (works for any type supporting operator*, operator+=, etc.) */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Find the diagonal d satisfying d*(d+1)/2 >= n */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 prod = (__int128)mid * (mid + 1) / 2;
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;

    /* Number of elements before this diagonal */
    __int128 prev128 = (__int128)d * (d - 1) / 2;
    idx_t prev = (idx_t)prev128;

    /* Position inside diagonal (0‑based) */
    idx_t offset = n - prev - 1;

    /* Coordinates (row i, column j) */
    idx_t i = offset + 1;        /* 1‑based row */
    idx_t j = d - i;             /* 0‑based column (k) */

    /* Compute A(i,j) multiplicatively */
    seq_t result = 1;
    idx_t temp = i;

    /* Trial division factorisation of i (small numbers, fast) */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* term = p^(j*(e-1)) * (p^j + 1) */
            seq_t p_pow_j = pow_seq((seq_t)p, j);
            seq_t part = p_pow_j + 1;
            if (e > 1) {
                seq_t p_pow_ex = pow_seq((seq_t)p, j * (e - 1));
                part *= p_pow_ex;
            }
            result *= part;
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        idx_t p = temp;
        idx_t e = 1;
        seq_t p_pow_j = pow_seq((seq_t)p, j);
        seq_t part = p_pow_j + 1;
        result *= part;
    }

    return result;
}
