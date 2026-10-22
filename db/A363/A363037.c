/*
 * A363037  Expansion of Sum_{k>0} x^k / (1 + x^(4*k)).
 * https://oeis.org/A363037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A363037: a(n) = sum_{d|n, d≡1 (mod 4)} (-1)^((d-1)/4) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1; undefined for 0 */
    seq_t pos = 0;                  /* sum of +1 contributions   */
    seq_t neg = 0;                  /* sum of -1 contributions   */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            if ((d & 3ULL) == 1ULL)          /* d ≡ 1 (mod 4) */
            {
                idx_t exp = (d - 1) / 4;     /* ((d-1)/4) */
                if ((exp & 1ULL) == 0ULL)    /* even exponent -> +1 */
                    pos++;
                else
                    neg++;
            }

            /* corresponding divisor j = n/d, skip if same as d */
            idx_t j = n / d;
            if (j != d && ((j & 3ULL) == 1ULL))
            {
                idx_t exp = (j - 1) / 4;
                if ((exp & 1ULL) == 0ULL)
                    pos++;
                else
                    neg++;
            }
        }
    }
    /* pos ≥ neg for all n (empirical evidence), so subtraction is safe */
    return pos - neg;
}
