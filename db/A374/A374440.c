/*
 * A374440  Triangle read by rows: T(n, k) = T(n - 1, k) + T(n - 2, k - 2),
 *          with boundary conditions: if k = 0 or k = 2 then T = 1; if k = 1
 *          then T = n - 1.
 * https://oeis.org/A374440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2 and
 *            c = n – r(r+1)/2. Triangle rule: T(n,0) = 1, T(n,1) = n-1,
 *            T(n,2) = 1 T(n,k) = T(n‑1,k) + T(n‑2,k‑2) for k > 2 Closed form
 *            (used when k > 2): T(n,k) = binomial(n – ⌊k/2⌋, ⌈k/2⌉) –
 *            binomial(n – ⌈(k+even(k))/2⌉, ⌊k/2⌋)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

static inline idx_t row_of(idx_t n)
{
    /* Small r (~140 for n up to 10000), linear search is trivial. */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n) ++r;
    return r - 1;
}

static inline seq_t T_value(idx_t n, idx_t k)
{
    if (k == 0 || k == 2) return (seq_t)1;
    if (k == 1) return (seq_t)(n - 1);

    /* k > 2, use the closed form */
    idx_t floor_k2 = k >> 1;                 /* floor(k/2)   */
    idx_t ceil_k2  = (k + 1) >> 1;           /* ceil(k/2)    */
    bool   even_k  = ((k & 1) == 0);         /* 1 if even    */
    idx_t  t       = k + (even_k ? 1 : 0);
    idx_t  ceil_t2 = (t + 1) >> 1;           /* ceil((k+even)/2) */

    idx_t a1 = n - floor_k2;
    idx_t b1 = ceil_k2;
    idx_t a2 = n - ceil_t2;
    idx_t b2 = floor_k2;

    seq_t term1 = binom_calc(a1, b1);
    seq_t term2 = binom_calc(a2, b2);
    return term1 - term2;                    /* guaranteed non‑negative */
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t start = (r * (r + 1)) / 2;
    idx_t c = n - start;
    return T_value(r, c);
}
