/*
 * A316622  Array read by antidiagonals: T(n,k) is the order of the group
 *          GL(n,Z_k).
 * https://oeis.org/A316622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, tabl
 * Formula:   a(n) = T(row,col) read by antidiagonals, where T(n,k) =
 *            |GL(n,ℤ_k)| = ∏_{p^e||k} [ p^{e·n² - n(n+1)/2} · ∏_{j=1}^n (p^j
 *            - 1) ]. The index n is read by first enumerating the array T
 *            with (row=n, col=k) in antidiagonal order starting at (0,1)
 * Range:     u8 n <= 17, u16 n <= 23, u32 n <= 32, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   _floor_isqrt_u64, factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* Convert global index to the (row,col) coordinates used for GL(n,k). */
static void idx_to_coords(idx_t idx, idx_t *row, idx_t *col)
{
    /* Find the diagonal s such that (s-1)*s/2 <= idx < s*(s+1)/2. */
    idx_t tmp = 8 * idx + 1;
    idx_t r   = _floor_isqrt_u64(tmp);
    idx_t s   = (r - 1) / 2 + 1;          /* diagonal number (starting at 1) */

    /* Offset within the diagonal. */
    idx_t prev = (s - 1) * s / 2;
    idx_t j    = idx - prev;              /* j = row index within the diagonal */

    *row = j;
    *col = s - j;
}

/* Compute T(n,k) = |GL(n,ℤ_k)|.  Assumes n >= 0 and k >= 1. */
static seq_t T_n_k(idx_t n, idx_t k)
{
    if (k == 1) {
        /* GL(n,ℤ_1) has only the 0×0 matrix for n=0 and one matrix for n>0. */
        return 1;
    }

    /* Factor k into primes.  k <= 2^64 so a small array suffices. */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)k, pr, ex, &cnt);

    seq_t result = 1;

    for (size_t fi = 0; fi < cnt; ++fi) {
        uint64_t p  = pr[fi];
        uint64_t e  = ex[fi];

        /* Compute the exponent part: p^{e·n² - n(n+1)/2}. */
        uint64_t exp_diff = e * (n * n) - (n * (n + 1) / 2);
        seq_t pow_part = seq_pow((seq_t)p, exp_diff);

        /* Compute the product over j = 1..n of (p^j - 1). */
        seq_t prod = 1;
        for (idx_t j = 1; j <= n; ++j) {
            seq_t pj   = seq_pow((seq_t)p, j);
            seq_t term = pj - 1;              /* pj-1 fits in seq_t */
            prod = seq_mul(prod, term);
        }

        seq_t term = seq_mul(pow_part, prod);
        result = seq_mul(result, term);
    }

    return result;
}

seq_t a(idx_t idx)
{
    idx_t n, k;
    idx_to_coords(idx, &n, &k);
    return T_n_k(n, k);
}
