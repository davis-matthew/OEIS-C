/*
 * A396322  Expansion of e.g.f. exp(x)*(cosh(x)-1)*(sinh(x)-x).
 * https://oeis.org/A396322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n - (-1)^n)/4 - (n + 2)*2^(n-2) + n, with
 *            a(0)=a(1)=a(2)=a(3)=a(4)=0 (formula holds for n≥2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast power with an idx_t exponent – works for any seq_t (8‑, 16‑, 32‑, 64‑ or 128‑bit) */
static seq_t pow_seq_u64(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1ULL) result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* The given formula yields 0 for 0 ≤ n<5 */
    if (n < 5) return 0;

    /* Powers needed by the formula */
    seq_t pow3 = pow_seq_u64((seq_t)3, n);        /* 3^n      */
    seq_t pow2 = pow_seq_u64((seq_t)2, n - 2);    /* 2^(n‑2)  */

    /* (3^n - (-1)^n)/4  */
    seq_t term1;
    if (n & 1ULL)                /* n is odd ⇒ (-1)^n = –1 */
        term1 = (pow3 + 1) / 4;
    else                         /* n is even ⇒ (-1)^n = +1 */
        term1 = (pow3 - 1) / 4;

    /* (n + 2) * 2^(n‑2) */
    seq_t term2 = (seq_t)(n + 2ULL) * pow2;

    /* Final result */
    return term1 - term2 + (seq_t)n;
}
