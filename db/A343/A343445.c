/*
 * A343445  Coefficients of the series S(p, q) for which (-sqrt(p))*S
 *          converges to the largest real root of x^3 - p*x + q for 0 < p and
 *          0 < q < 2*(p/3)^(3/2).
 * https://oeis.org/A343445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*(3*n-5)*(3*n-7)*a(n-2),
   with a(0) = -1, a(1) = 1, a(2) = 3. */

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* Last computed values for each parity.
       even_prev / even_val correspond to even indices (0,2,4,...);
       odd_prev  / odd_val  correspond to odd  indices (1,3,5,...). */
    seq_t even_prev = (seq_t)3;  /* a(2) */
    seq_t odd_prev  = (seq_t)1;  /* a(1) */
    seq_t even_val = even_prev;  /* current a(2) */
    seq_t odd_val  = odd_prev;   /* current a(1) */

    /* Iterate from 3 up to n, updating only the parity that matches i. */
    for (idx_t i = 3; i <= n; ++i)
    {
        /* coeff = 3*(3*i-5)*(3*i-7)  (computed entirely in seq_t) */
        seq_t coeff = (seq_t)3 *
                      ( (seq_t)(3*i - 5) ) *
                      ( (seq_t)(3*i - 7) );

        if (i & 1)          /* odd index */
        {
            odd_val  = coeff * odd_prev;
            odd_prev = odd_val;
        }
        else                /* even index */
        {
            even_val  = coeff * even_prev;
            even_prev = even_val;
        }
    }

    return (n & 1) ? odd_val : even_val;
}
