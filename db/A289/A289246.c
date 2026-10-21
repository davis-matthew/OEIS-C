/*
 * A289246  Coefficients in the expansion of 1/Sum_{k >= 0} ([r*(k + 1)] +
 *          [s*(k + 1)]) * (-x)^k, where [ ] = floor, r = (1+sqrt(5))/2, s =
 *          1/r.
 * https://oeis.org/A289246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the coefficient of xⁿ in the power series 1 / Σ_{k≥0} (
 *            ⌊φ(k+1)⌋ + ⌊(1/φ)(k+1)⌋ ) (‑x)ᵏ , φ = (1+√5)/2. Recurrence: a(0)
 *            = 1 a(n) = Σ_{k=1}^{n} (‑1)^{k+1} cₖ a(n–k), where cₖ = ⌊φ(k+1)⌋
 *            + ⌊(1/φ)(k+1)⌋ = 2 ⌊φ(k+1)⌋ – (k+1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* integer floor square‑root of a 64‑bit unsigned integer */
static uint64_t int_sqrt_u64(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = 1ULL << 62;          /* highest power of 4 ≤ 2⁶⁴‑1 */
    while (bit > x) bit >>= 2;
    for (; bit; bit >>= 2)
    {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else
            r >>= 1;
    }
    return r;
}

/* ⌊n·φ⌋ where φ=(1+√5)/2, computed with pure integer arithmetic */
static seq_t floor_phi_mul(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t m = 5 * nn * nn;          /* 5·n² fits in 64 bits for n≤10⁴  */
    uint64_t s = int_sqrt_u64(m);      /* floor(√(5n²)) = floor(n·√5)  */
    return (seq_t)((nn + s) / 2);      /* floor((n+n√5)/2) = ⌊nφ⌋     */
}

/* cₖ = 2·⌊φ(k+1)⌋ – (k+1) */
static seq_t ck_at(idx_t k)
{
    idx_t n = k + 1;
    seq_t t = floor_phi_mul(n);
    return 2 * t - (seq_t)n;
}

/* cache for the sequence values */
static seq_t *a_cache = NULL;
static size_t a_cap   = 0;
static size_t a_len   = 0;

/* fill the cache up to index n */
static void ensure_cache(idx_t n)
{
    if (a_cap <= (size_t)n)
    {
        size_t new_cap = a_cap ? a_cap * 2 : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }

    if (a_len == 0)          /* first time: set a(0) */
    {
        a_cache[0] = (seq_t)1;
        a_len = 1;
    }

    for (idx_t i = a_len; i <= n; ++i)
    {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k)
        {
            seq_t ck   = ck_at(k);
            seq_t prod = ck * a_cache[i - k];
            if (k & 1)      /* odd k → plus */
                sum += prod;
            else            /* even k → minus */
                sum -= prod;
        }
        a_cache[i] = sum;
    }
    a_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
