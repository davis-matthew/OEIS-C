/*
 * A243284  a(n) = the number of distinct ways of writing such products m =
 *          k^2 * j, 0 < j <= k, (j and k natural numbers) that m is in range
 *          [1,n]; Partial sums of A102354.
 * https://oeis.org/A243284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{⌊√n⌋} min(k, ⌊n/k²⌋)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function counts all pairs (k, j) with 1 ≤ j ≤ k and k²·j ≤ n. */

static inline idx_t int_sqrt(idx_t n)
{
    /* Integer square root by binary search. */
    idx_t low = 0, high = n;
    while (low < high)
    {
        idx_t mid = (low + high + 1) / 2;  /* upper mid to avoid infinite loop */
        if (mid <= n / mid)                /* mid*mid <= n without overflow */
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0)
        return 0;                       /* for safety, though n ≥ 1 in specification */

    idx_t maxk = int_sqrt(n);
    for (idx_t k = 1; k <= maxk; ++k)
    {
        idx_t prod = k * k;              /* k², fits in idx_t because k ≤ √n */
        idx_t div  = n / prod;           /* floor(n / k²) */
        seq_t add = (div < k) ? (seq_t)div : (seq_t)k;
        sum += add;
    }
    return sum;
}
