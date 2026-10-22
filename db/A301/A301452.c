/*
 * A301452  Number of ways to write n^2 as m*4^k + x^2 + 2*y^2 with m in the
 *          set {2, 3} and k,x,y nonnegative integers.
 * https://oeis.org/A301452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered quadruples (m,k,x,y) with m ∈ {2,3},
 *            k,x,y ∈ ℕ₀ such that n² = m·4ᵏ + x² + 2·y². The function
 *            enumerates all k and counts solutions of x² + 2·y² = N for each
 *            remainder N = n² − m·4ᵏ. Counting x² + 2·y² is done by checking
 *            all x ≤ √N and testing whether (N − x²)/2 is a per...
 * Range:     u8 n <= 15, u16 n <= 422, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Floor square‑root for 64‑bit unsigned integers using binary search.
   The result never overflows 64‑bit. */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0;
    uint64_t high = (uint64_t)1 << 32;      /* sqrt(2¹⁶⁴) < 2³² */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Test whether v is a perfect square (v ≤ UINT64_MAX). */
static inline int is_square_u64(uint64_t v)
{
    uint64_t r = isqrt_u64(v);
    return r * r == v;
}

/* Count non‑negative integer solutions to x² + 2·y² = N (N ≤ UINT64_MAX). */
static inline seq_t count_x2_2y2(seq_t N)
{
    /* N fits into 64 bits for the intended range (n ≤ 10000). */
    uint64_t n = (uint64_t)N;
    uint64_t lim = isqrt_u64(n);
    seq_t cnt = 0;
    for (uint64_t x = 0; x <= lim; ++x) {
        uint64_t R = n - x * x;
        if (R & 1ULL)            /* 2·y² must be even */
            continue;
        uint64_t y2 = R >> 1;    /* y² = R/2 */
        if (is_square_u64(y2))
            ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset starts at 1 */
    seq_t total = 0;
    seq_t n2 = (seq_t)n * (seq_t)n;       /* n² */

    /* Enumerate m ∈ {2,3} */
    for (int m_int = 2; m_int <= 3; ++m_int) {
        seq_t m = (seq_t)m_int;
        seq_t limit = n2 / m;             /* maximum 4ᵏ such that m·4ᵏ ≤ n² */

        seq_t pow4 = 1;                   /* 4⁰ */
        while (pow4 <= limit) {
            seq_t rem = n2 - m * pow4;    /* n² - m·4ᵏ */
            total += count_x2_2y2(rem);

            /* Stop before overflow: check if next 4ᵏ would exceed limit. */
            if (pow4 > (limit >> 2))
                break;
            pow4 <<= 2;                  /* multiply by 4 */
        }
    }
    return total;
}
