/*
 * A372613  Number of solutions to x^2 + y^2 + z^2 + w^2 <= n, where x, y, z,
 *          w are positive odd integers.
 * https://oeis.org/A372613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered 4‑tuples (x,y,z,w) of positive odd
 *            integers such that x² + y² + z² + w² ≤ n
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* helper: integer square root, floor(sqrt(n))   */
static idx_t int_isqrt(idx_t n)
{
    idx_t r = 0;
    while ((idx_t)(r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* compute list of odd squares ≤ n */
    idx_t max_root = int_isqrt(n);
    idx_t max_odd = (max_root & 1) ? max_root : max_root - 1;
    if (max_odd < 1) return 0; /* no odd numbers possible */

    idx_t num_odds = (max_odd + 1) / 2;          /* 1,3,5,…,max_odd */
    uint64_t *odd_sq = (uint64_t *)malloc(num_odds * sizeof(uint64_t));
    if (!odd_sq) return 0;
    for (idx_t i = 0; i < num_odds; ++i) {
        idx_t val = 1 + 2 * i;
        odd_sq[i] = (uint64_t)val * (uint64_t)val;
    }

    /* count ordered pairs (x,y) such that x² + y² = s */
    size_t sz = (size_t)n + 1;
    uint64_t *cnt2 = (uint64_t *)calloc(sz, sizeof(uint64_t));
    if (!cnt2) { free(odd_sq); return 0; }

    for (idx_t i = 0; i < num_odds; ++i) {
        uint64_t a_sq = odd_sq[i];
        for (idx_t j = 0; j < num_odds; ++j) {
            uint64_t sum = a_sq + odd_sq[j];
            if (sum <= n) cnt2[sum] += 1;
        }
    }

    /* prefix sums of cnt2 */
    uint64_t *pref = (uint64_t *)malloc(sz * sizeof(uint64_t));
    if (!pref) { free(odd_sq); free(cnt2); return 0; }

    pref[0] = cnt2[0];
    for (size_t i = 1; i < sz; ++i)
        pref[i] = pref[i-1] + cnt2[i];

    /* total number of 4‑tuples */
    uint64_t result = 0;
    for (size_t s = 0; s < sz; ++s) {
        size_t rem = sz - 1 - s;
        result += cnt2[s] * pref[rem];
    }

    free(odd_sq);
    free(cnt2);
    free(pref);
    return (seq_t)result;
}
