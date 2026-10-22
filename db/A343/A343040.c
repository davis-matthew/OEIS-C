/*
 * A343040  Array T(n, k), n, k >= 0, read by antidiagonals; lunar addition
 *          table for the factorial base.
 * https://oeis.org/A343040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the coordinates of the n‑th
 *            element in the antidiagonal traversal of the lunar
 *            factorial‑base table T. T(i,j) is defined by taking the
 *            factorial‑base digits of i and j and putting at each position
 *            the maximum of the two digits
 * Range:     u8 n <= 9559, u16 n <= 9559, u32 n <= 9559, u64 n <= 9559,
 *            u128 n <= 9559, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal index s such that
       s*(s+1)/2 <= n < (s+1)*(s+2)/2  */
    idx_t s = 0;
    while ((s * (s + 1)) / 2 <= n)
        ++s;
    --s;                                   /* now s*(s+1)/2 <= n */

    idx_t t_prev = (s * (s + 1)) / 2;     /* terms before diagonal s   */
    idx_t k = n - t_prev;                  /* offset inside this diagonal (0 <= k <= s) */
    idx_t i = k;                           /* row index                 */
    idx_t j = s - k;                       /* column index              */

    /* Compute T(i,j) via factorial‑base digitwise maximum */
    idx_t temp_i = i, temp_j = j;
    seq_t result = 0;
    seq_t weight  = 1;                      /* 1! initially            */
    for (idx_t p = 1; ; ++p)
    {
        idx_t divisor = p + 1;              /* next factorial divisor   */
        idx_t di = temp_i % divisor;
        idx_t dj = temp_j % divisor;
        idx_t d  = (di > dj) ? di : dj;    /* max digit for this place */

        result += (seq_t)d * weight;        /* add contribution         */
        weight *= divisor;                  /* next factorial weight   */

        temp_i /= divisor;
        temp_j /= divisor;
        if (temp_i == 0 && temp_j == 0)
            break;
    }
    return result;
}
