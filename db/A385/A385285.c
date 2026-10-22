/*
 * A385285  a(n) = Sum_{k=0..n} (binomial(n, k) mod 8).
 * https://oeis.org/A385285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (C(n,k) mod 8)
 * Range:     u8 n <= 62, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline int v2_uint64(idx_t x)
{
    return x ? __builtin_ctzll((unsigned long long)x) : 0;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;           /* running sum of binomial mod 8 */
    seq_t odd_part = 1;      /* odd part of the current binomial coefficient (mod 8) */
    int exp2 = 0;            /* exponent of 2 in the current binomial coefficient */

    /* k = 0 term */
    sum += 1;

    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t num = n - k + 1;   /* numerator of the k-th term of the multiplicative form */
        idx_t den = k;           /* denominator of the k-th term */

        int num2 = v2_uint64(num);  /* exponent of 2 in numerator */
        int den2 = v2_uint64(den);  /* exponent of 2 in denominator */

        exp2 += num2 - den2;        /* update exponent of 2 */

        idx_t odd_num = num >> num2; /* odd part of numerator */
        idx_t odd_den = den >> den2; /* odd part of denominator */

        odd_part = (odd_part * (seq_t)(odd_num & 7)) & 7;
        odd_part = (odd_part * (seq_t)(odd_den & 7)) & 7;

        seq_t add;
        if (exp2 >= 3) {
            add = 0;                     /* 2^exp2 contributes 0 modulo 8 when exp2 ≥ 3 */
        } else {
            seq_t mul2 = (seq_t)1 << exp2; /* exp2 is 0,1,or 2 here */
            add = (odd_part * mul2) & 7;
        }
        sum += add;
    }

    return sum;
}
