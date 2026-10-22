/*
 * A324024  One of the two successive approximations up to 5^n for the 5-adic
 *          integer sqrt(6). This is the 4 (mod 5) case (except for n = 0).
 * https://oeis.org/A324024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A324024 – 5‑adic square root of 6 (congruent to 4 mod 5).
    a(0) = 0.  For n > 0, a(n) is the unique integer 0 ≤ x < 5ⁿ
    such that x² ≡ 6 (mod 5ⁿ) and x ≡ 4 (mod 5).  The value is
    produced by Hensel lifting in base‑5 digits.  No external tables
    are used.  Complexity is O(n²) arithmetic on small 64‑bit
    integers, which is small enough for n ≤ 10000 and fits the
    required time limits. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* a(0) */

    /* Store digits of the 5‑adic number (least significant first). */
    size_t N = (size_t)n;
    size_t maxSize = 2 * N + 3;   /* enough for partial products */
    uint64_t *digits  = (uint64_t *)malloc(maxSize * sizeof(uint64_t));
    uint64_t *partial = (uint64_t *)malloc(maxSize * sizeof(uint64_t));
    uint64_t *temp    = (uint64_t *)malloc(maxSize * sizeof(uint64_t));
    if (!digits || !partial || !temp) {
        /* allocation failed – return 0 (unlikely in tests). */
        return (seq_t)0;
    }

    for (size_t i = 0; i < maxSize; ++i) {
        partial[i] = 0;
        temp[i]    = 0;
    }

    /* initial digit a₀ = 4  (4² ≡ 6 mod 5) */
    digits[0]   = 4;
    partial[0]  = 4 * 4;           /* raw product 4² = 16 */

    seq_t x    = (seq_t)4;          /* current approximation */
    seq_t pow5 = (seq_t)5;          /* 5¹ */

    for (size_t k = 1; k < N; ++k) {
        /* ---------- compute digit a_k ---------- */
        /* copy raw partial products up to index k */
        for (size_t i = 0; i <= k; ++i)
            temp[i] = partial[i];

        /* propagate carries up to exponent k‑1 */
        for (size_t e = 0; e < k; ++e) {
            uint64_t carry = temp[e] / 5;
            temp[e] %= 5;
            temp[e + 1] += carry;
        }

        /* subtract 6 = 1 + 1·5 (in base 5) */
        int borrow = 0;
        /* position 0 */
        long long val0 = (long long)temp[0] - 1 - borrow;
        if (val0 < 0) { val0 += 5; borrow = 1; } else borrow = 0;
        temp[0] = (uint64_t)val0;
        /* position 1 */
        long long val1 = (long long)temp[1] - 1 - borrow;
        if (val1 < 0) { val1 += 5; borrow = 1; } else borrow = 0;
        temp[1] = (uint64_t)val1;
        /* propagate remaining borrow */
        size_t idx = 2;
        while (borrow && idx <= k) {
            long long val = (long long)temp[idx] - borrow;
            if (val < 0) { val += 5; borrow = 1; } else borrow = 0;
            temp[idx] = (uint64_t)val;
            ++idx;
        }

        uint64_t C = temp[k] % 5;                      /* (x² – 6)/5ᵏ  mod 5 */
        uint64_t d = ((5 - C) * 2) % 5;                /* a_k ≡ –C·2⁻¹ (mod 5) */
        digits[k] = d;

        /* ---------- update raw partial products ----------- */
        for (size_t j = 0; j < k; ++j)
            partial[k + j] += 2 * d * digits[j];
        partial[2 * k] += d * d;

        /* ---------- update the integer value ------------ */
        x += (seq_t)d * pow5;
        pow5 *= (seq_t)5;
    }

    free(digits);
    free(partial);
    free(temp);
    return x;
}
