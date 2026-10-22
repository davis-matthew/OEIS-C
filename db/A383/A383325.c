/*
 * A383325  Numbers not of the form round(3^k/5). Complement of A383324.
 * https://oeis.org/A383325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m+1 if m >= floor((3^k+2)/5) else a(n) = m where k =
 *            floor(log_3(5*n+8))+1 m = n-1+k The function works for all n >=
 *            1 where the result fits in seq_t
 * Range:     u8 n <= 46, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the minimum index (offset is 1). */
    if (n == 0) {
        return 0; /* Undefined for n==0 according to offset, return 0. */
    }

    /* Compute x = 5*n + 8, all in seq_t to avoid overflow of idx_t. */
    seq_t x = (seq_t)5 * n + 8;

    /* Find e = floor(log_3(x)). */
    idx_t e = 0;
    seq_t pow = 1;                /* pow = 3^e */
    seq_t thr;

    while (pow <= x / 3) {        /* safe mult: pow*3 <= x */
        pow *= 3;
        e++;
    }

    idx_t k = e + 1;              /* k = floor(log_3(x)) + 1 */

    seq_t pow_k = pow * 3;        /* 3^k = 3^(e+1) */
    thr = (pow_k + 2) / 5;        /* floor((3^k + 2)/5) */

    seq_t m = (seq_t)n - (seq_t)1 + (seq_t)k; /* m = n - 1 + k */

    if (m >= thr)
        return m + 1;
    else
        return m;
}
