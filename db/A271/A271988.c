/*
 * A271988  g_n(7) where g is the weak Goodstein function defined in A266202.
 * https://oeis.org/A271988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A271988 – g_n(7) where g is the weak Goodstein function
    Formula:  a(0) = 7
              a(n) = b_{n+1}(a(n-1)) – 1   (n ≥ 1)
    where b_k(x) interprets the base‑k expansion of x as a number in base (k+1). */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)7;

    seq_t cur = (seq_t)7;

    for (idx_t i = 1; i <= n; ++i) {
        /* base b = i+1 is used for the current step */
        seq_t base   = (seq_t)(i + 1);
        seq_t factor = base + 1;          /* base (b+1) for the evaluation */

        seq_t temp   = cur;
        seq_t power  = 1;                 /* (b+1)^k during reconstruction */
        seq_t sum    = 0;                 /* reconstructed value before –1 */

        while (temp > 0) {
            seq_t digit = temp % base;    /* current digit in base b */
            temp /= base;
            sum += digit * power;        /* digit times (b+1)^k */
            power *= factor;             /* advance power of (b+1) */
        }

        cur = sum - 1;                    /* a(i) = b_{i+1}(a(i‑1)) – 1 */
    }

    return cur;
}
