/*
 * A276755  a(n) = A275706(n)^2 + A276688(n)^2 = [n]_{1+i}! * [n]_{1-i}!,
 *          where [n]_q! is the q-factorial, i = sqrt(-1).
 * https://oeis.org/A276755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |[n]_{1+i}!|^2 = ∏_{k=1}^n |1-(1+i)^k|^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 0, a(0) = 1.  The product can be written as a product of
   integer factors:  factor_k = 1 + 2^k − 2·Re((1+i)^k).  Re((1+i)^k)
   takes only integer values depending on k mod 8, as shown below.  */

seq_t a(idx_t n)
{
    seq_t result = 1;          /* a(0) = 1 */
    /* Maximum safe shift for fixed-width integer types. */
    const unsigned int bits = sizeof(seq_t) * 8;
    const unsigned int max_shift = (bits == 0) ? 0 : bits - 1;  /* 0 for empty type */

    for (idx_t k = 1; k <= n; ++k) {
        /* If the shift amount would be undefined for a fixed-width type,
           we stop the computation; such values do not fit in the type
           and are not required for the 64-bit build. */
        if (k > max_shift) break;

        seq_t pow2k = (seq_t)1 << k;          /* 2^k */

        seq_t abs_t;       /* |Re((1+i)^k)| as a power of two */
        int  sign;         /* +1 or -1 (0 only for k%8==2 or 6) */

        switch (k & 7) {   /* k modulo 8 */
            case 0:
                abs_t = (seq_t)1 << (k / 2);
                sign  =  1;
                break;
            case 1: case 7:
                abs_t = (seq_t)1 << ((k - 1) / 2);
                sign  =  1;
                break;
            case 2: case 6:
                abs_t = 0;
                sign  =  0;
                break;
            case 3: case 5:
                abs_t = (seq_t)1 << ((k - 1) / 2);
                sign  = -1;
                break;
            case 4:
                abs_t = (seq_t)1 << (k / 2);
                sign  = -1;
                break;
            default:          /* unreachable */
                abs_t = 0;
                sign  = 0;
                break;
        }

        seq_t base   = pow2k + 1;          /* 1 + 2^k  */
        seq_t factor;
        if (abs_t == 0)                   /* Re((1+i)^k) = 0  */
            factor = base;
        else if (sign > 0)
            factor = base - (seq_t)2 * abs_t;
        else
            factor = base + (seq_t)2 * abs_t;

        result *= factor;                  /* a(k) = a(k-1) * factor_k */
    }

    return result;
}
