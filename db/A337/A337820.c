/*
 * A337820  Array read by antidiagonals: T(n,k) (n >= 1, k >= 0) is the ratio
 *          (the number of nonnegative bases m < n such that m^k == m (mod
 *          n))/(the number of nonnegative bases m < n such that -m^k == m
 *          (mod n)).
 * https://oeis.org/A337820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the ratio T(n,k) where T(n,k) = # {m ∈ [0,n-1] | m^k ≡ m
 *            (mod n)} ------------------------------------------------
 *            ---------------------------------------- / # {m ∈ [0,n-1] | -m^k
 *            ≡ m (mod n)}
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   antidiagonal, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required headers */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/antidiagonal.h"

/* compute the sequence term a(n) */
seq_t a(idx_t idx)
{
    idx_t n_val, k_val;
    antidiagonal(idx, &n_val, &k_val);

    seq_t num = 0;
    seq_t den = 0;

    for (idx_t m = 0; m < n_val; ++m) {
        seq_t val = mod_pow((seq_t)m, k_val, (seq_t)n_val);
        if (val == (seq_t)m)
            ++num;
        /* -m^k ≡ m (mod n)  ⟺  m^k + m ≡ 0 (mod n) */
        if (((val + (seq_t)m) % (seq_t)n_val) == 0)
            ++den;
    }
    /* den is always > 0 for n ≥ 1, so this division is exact */
    return den ? (num / den) : 0;
}
