/*
 * A321416  Number of n element multisets of the 10th roots of unity with zero
 *          sum.
 * https://oeis.org/A321416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{m ≡ 3n (mod 5), 0 ≤ m ≤ ⌊n/2⌋} C(m+4,4) * (1 if n = 2m
 *            else 2) (C(k,4) = k*(k-1)*(k-2)*(k-3)/24)
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 523, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* compute binomial coefficient C(k,4) for k ≥ 4 using 128‑bit arithmetic
   to avoid intermediate overflow. */
static inline seq_t binom4(idx_t k)
{
    /* k ≥ 4 is always true in our use. */
    unsigned __int128 t = 1;
    t *= (unsigned long long)(k);
    t *= (unsigned long long)(k - 1);
    t *= (unsigned long long)(k - 2);
    t *= (unsigned long long)(k - 3);
    t /= 24;
    return (seq_t)t;
}

/* main sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* m runs over all integers such that n - 2m is divisible by 5. */
    /* Solve 2m ≡ n (mod 5) → m ≡ 3n (mod 5). */
    const idx_t residue = (idx_t)((3 * (n % 5)) % 5);
    idx_t max_m = n / 2;
    for (idx_t m = residue; m <= max_m; m += 5) {
        seq_t coeff = binom4(m + 4); /* C(m+4,4) */
        if (n == 2 * m) {
            /* h(k) = 1 when k = n-2m = 0 */
            sum += coeff;
        } else {
            /* h(k) = 2 for all other multiples of 5 (k > 0) */
            sum += coeff * 2;
        }
    }
    return sum;
}
