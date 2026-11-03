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
 * Validated: partial, pass, 97 terms through n = 97 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required headers */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"

/* convert 1‑based index to the pair (n,k) in the antidiagonal read\n
 * of the infinite array.  For a given index idx we find the smallest
 * s with s*(s+1)/2 ≥ idx.  Then idx lies in row s and
 * offset = idx - s*(s-1)/2 - 1 (0‑based within the row).
 * The row contains the terms (n=s, k=0), (s-1,1), … , (1,s-1);
 * hence n = s - offset, k = offset. */
static void antidiagonal(idx_t idx, idx_t *n, idx_t *k)
{
    /* binary search for minimal s with s*(s+1)/2 >= idx */
    idx_t low = 1, high = 1;
    while (((__int128)high * (high + 1) / 2) < idx)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (((__int128)mid * (mid + 1) / 2) >= idx)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;
    idx_t prev = s * (s - 1) / 2;
    idx_t offset = idx - prev - 1;          /* 0‑based offset in row s */
    *n = s - offset;                        /* n decreases along the row */
    *k = offset;                            /* k increases */
}

/* compute the sequence term a(n) */
seq_t a(idx_t idx)
{
    idx_t n_val, k_val;
    antidiagonal(idx, &n_val, &k_val);

    seq_t num = 0;
    seq_t den = 0;

    for (idx_t m = 0; m < n_val; ++m) {
        seq_t val = powmod((seq_t)m, k_val, (seq_t)n_val);
        if (val == (seq_t)m)
            ++num;
        /* -m^k ≡ m (mod n)  ⟺  m^k + m ≡ 0 (mod n) */
        if (((val + (seq_t)m) % (seq_t)n_val) == 0)
            ++den;
    }
    /* den is always > 0 for n ≥ 1, so this division is exact */
    return den ? (num / den) : 0;
}
