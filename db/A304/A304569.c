/*
 * A304569  Triangle read by rows: T(n,k) = 1 if k | n^e with e >= 0,
 *          otherwise T(n,k) = 0 (1 <= k <= n).
 * https://oeis.org/A304569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9040, u16 n <= 9040, u32 n <= 9040, u64 n <= 9040,
 *            u128 n <= 9040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A304569: a(n) is the flattened triangle read by rows.
   For the integer n>=1 find the row r and column k so that
   n = r(r-1)/2 + k   (1 ≤ k ≤ r).  Then a(n) = 1 iff every prime
   factor of k also divides r; equivalently iff rad(k) | r.
   This uses only integer arithmetic. */
seq_t a(idx_t n)
{
    if (n == 0)         /* undefined, return 0 */
        return (seq_t)0;

    /* find row r such that r(r-1)/2 < n ≤ r(r+1)/2 */
    idx_t r = 1;
    idx_t cum = 1;            /* cumulative count up to current row */
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* column index k (1‑based) */
    idx_t prevCum = cum - r;
    idx_t k = n - prevCum;

    if (k == 1)           /* 1 always divides r^e */
        return (seq_t)1;

    /* test whether every distinct prime factor of k divides r */
    idx_t temp = k;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            if (r % p != 0)
                return (seq_t)0;      /* prime factor not in r */
            while (temp % p == 0)
                temp /= p;            /* remove powers of p */
        }
    }
    if (temp > 1) {      /* remaining prime factor > sqrt(original k) */
        if (r % temp != 0)
            return (seq_t)0;
    }

    return (seq_t)1;
}
