/*
 * A297250  Numbers whose base-3 digits having equal up-variation and
 *          down-variation; see Comments.
 * https://oeis.org/A297250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth natural number whose base‑3 representation starts and
 *            ends with the same digit. The first digit can be 1 or 2, any
 *            middle base‑3 digits are allowed. The sequence starts at n = 1.
 *            The algorithm finds the length L of the base‑3 representation of
 *            the desired number and computes the value...
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first two terms are trivial (1 and 2) */
    if (n <= 2)
        return (seq_t)n;

    /* n is at least 3 here */
    idx_t          L       = 1;      /* current length in base 3       */
    seq_t          pow     = 1;      /* pow = 3^(L-1)                  */
    seq_t          n_seq   = (seq_t)n;

    /* Find the smallest L such that 3^(L-1)+1 >= n   (cumulative count) */
    while (pow + 1 < n_seq) {
        pow *= 3;
        ++L;
    }

    /* Now pow = 3^(L-1).  Compute 3^(L-2) and the index within this block. */
    seq_t pow_prev = pow / 3;        /* 3^(L-2)  */
    seq_t k = n_seq - (pow_prev + 1); /* 1‑based index within length‑L numbers */

    /* the first 3^(L-2) numbers start with digit 1, the rest with digit 2 */
    seq_t d = 1;                      /* leading (and trailing) digit */
    if (k > pow_prev) {
        d = 2;
        k -= pow_prev;
    }

    /* middle block index (0‑based) */
    seq_t m = k - 1;                  /* 0 <= m < 3^(L-2) */

    /* Compose the final value:
       number = d * 3^(L-1) + middle_value * 3 + d */
    return d * pow + m * 3 + d;
}
