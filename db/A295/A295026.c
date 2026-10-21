/*
 * A295026  a(n) = floor(Pi*(n + sqrt(2))^2).
 * https://oeis.org/A295026
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊π·(n+√2)²⌋
 * Range:     u8 n <= 7, u16 n <= 143, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* High‑precision rational approximations
       π  ≈ 104348/33215   (error ≈ 8.5·10⁻¹⁰)
       √2 ≈ 1393/985      (error ≈ 1.5·10⁻¹⁰) */
    const unsigned long long PI_NUM   = 104348ULL;
    const unsigned long long PI_DEN   = 33215ULL;
    const unsigned long long SQR2_NUM = 1393ULL;
    const unsigned long long SQR2_DEN = 985ULL;
    const unsigned long long DENOM    = PI_DEN * SQR2_DEN * SQR2_DEN; /* 32226023375 */

    /* Compute (n + √2) with integer arithmetic */
    long long t_signed = (long long)n * (long long)SQR2_DEN + (long long)SQR2_NUM;
    unsigned long long t = (unsigned long long)t_signed;          /* t > 0 for n ≥ –1 */

    /* Compute floor( π · (t / SQR2_DEN)² ) */
    unsigned long long t_sq = t * t;                 /* (n*den + num)²  */
    unsigned long long num  = t_sq * PI_NUM;          /* numerator       */
    unsigned long long prod = num / DENOM;           /* floor division  */

    return (seq_t)prod;
}
