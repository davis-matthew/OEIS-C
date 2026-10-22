/*
 * A387657  Array read by ascending antidiagonals: A(n, k) =
 *          denominator(k^n/n), with k >= 0.
 * https://oeis.org/A387657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, tabl, changed
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A387657:  a(n) is the denominator of k^n / n where (n,k) is chosen from the array
   read by ascending antidiagonals.  For an index N ≥ 1:
   let s be the minimal integer with s*(s+1)/2 ≥ N.
   Let offset = N – (s−1)*s/2      (1 ≤ offset ≤ s).
   Then row n = s – offset + 1  and column k = offset – 1.
   The returned value is n / gcd(n, k^n);
   (for k = 0 the result is 1). */

seq_t a(idx_t N)
{
    /* --- find diagonal sum s --- */
    idx_t low = 1, high = 1;
    /* increase high until the triangular number exceeds N */
    while((__uint128_t)high * (high + 1) / 2 < N) high *= 2;
    /* binary search between low and high */
    while(low < high) {
        idx_t mid = low + (high - low) / 2;
        if((__uint128_t)mid * (mid + 1) / 2 >= N)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    /* position inside the diagonal */
    idx_t prev_tri = (idx_t)((__uint128_t)(s - 1) * s / 2);
    idx_t offset = N - prev_tri;      /* 1‑based inside diagonal */

    idx_t k   = offset - 1;           /* column index */
    idx_t row = s - offset + 1;       /* row index n */

    /* k = 0 gives 0/n = 0/1 */
    if(k == 0) return (seq_t)1;

    /* compute denominator = row / gcd(row, k^row) */
    /* this equals product of prime powers of row that do NOT divide k */
    seq_t denom = 1;

    idx_t temp = row;
    for(idx_t p = 2; p <= temp / p; ++p) {
        if(temp % p == 0) {
            idx_t exp = 0;
            while(temp % p == 0) { temp /= p; ++exp; }
            if(k % p != 0) {
                seq_t power = 1;
                for(idx_t i = 0; i < exp; ++i)
                    power *= (seq_t)p;
                denom *= power;
            }
        }
    }
    /* remaining prime factor, if any */
    if(temp > 1) {
        if(k % temp != 0)
            denom *= (seq_t)temp;
    }

    return denom;
}
