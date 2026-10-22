/*
 * A303481  Total volume of the family of rectangular prisms with dimensions
 *          p, q and p + q where p divides q, n = p + q and p < q.
 * https://oeis.org/A303481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Σ_{i=1..⌊(n-1)/2⌋} i*(n-i) * I[(n-i) % i == 0] where
 *            I[condition] is 1 if the condition holds and 0 otherwise
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    idx_t limit = (n - 1) / 2;         /* upper bound for i */
    seq_t sum = 0;                     /* Σ i*(n-i) for valid i */

    for (idx_t i = 1; i <= limit; ++i)
    {
        if ((n - i) % i == 0)          /* i divides n-i */
        {
            seq_t ii = (seq_t)i;                  /* i as seq_t */
            seq_t ni = (seq_t)(n - i);            /* n-i as seq_t */
            sum += ii * ni;                      /* accumulate */
        }
    }
    return (seq_t)n * sum;            /* final product */
}
