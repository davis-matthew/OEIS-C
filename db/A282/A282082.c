/*
 * A282082  Number of n-element subsets of [n+6] having an even sum.
 * https://oeis.org/A282082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k even} binom((n+6+1)/2, k) · binom((n+6)/2, n−k)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 131, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of n‑element subsets of [n+6] whose sum is even. */

#include <stdint.h>

/* Compute binomial coefficient C(a,b) for non‑negative a,b.
   The function uses only +, -, *, / and works for arbitrary‑precision seq_t. */
static seq_t
binom(idx_t a, idx_t b)
{
    if (b > a) return 0;
    if (b > a - b) b = a - b;
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res = res * (a - b + i) / i;
    }
    return res;
}

seq_t
a(idx_t n)
{
    idx_t N   = n + 6;                     /* size of the set          */
    idx_t O   = (N + 1) / 2;               /* odd elements            */
    idx_t E   = N / 2;                     /* even elements           */

    /* smallest k such that 0 ≤ k ≤ O, n−k ≤ E and k even */
    idx_t k_start = (n > E) ? n - E : 0;
    if (k_start & 1U) ++k_start;

    /* largest k with the same constraints */
    idx_t k_end = (n < O) ? n : O;
    if (k_end & 1U) --k_end;

    if (k_start > k_end) return 0;

    seq_t sum = 0;
    for (idx_t k = k_start; k <= k_end; k += 2U) {
        seq_t part_odd  = binom(O, k);
        seq_t part_even = binom(E, n - k);
        sum += part_odd * part_even;
    }
    return sum;
}
