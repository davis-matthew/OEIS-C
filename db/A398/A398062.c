/*
 * A398062  Array read by ascending antidiagonals: A(n,k) is the n-th
 *          nondivisor of k.
 * https://oeis.org/A398062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = the (row)-th nondivisor of k, where (row,k) is obtained
 *            from the index n by reading the infinite array A(row,k) = row-th
 *            nondivisor of k in reverse anti‑diagonal order
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth nondivisor of k. */
static seq_t nth_nondivisor(idx_t k, idx_t n)
{
    seq_t count = 0;
    seq_t x = 2;                     /* start checking from 2 */
    const seq_t kk = (seq_t)k;       /* k cast to seq_t for modulus */
    while (1) {
        if (kk % x != 0) {
            count++;
            if (count == n) return x;
        }
        x++;
    }
}

/* Main function as required. */
seq_t a(idx_t n)
{
    /* Find diagonal d such that T(d-1) < n <= T(d), where T(d)=d*(d+1)/2 */
    idx_t d = 1;
    idx_t tri = 1;       /* T(d) for current d */
    while (tri < n) {
        d++;
        tri += d;        /* T(d) = T(d-1) + d */
    }
    /* Offset within this diagonal, 0‑based. */
    idx_t prev_tri = tri - d;          /* T(d-1) */
    idx_t offset = n - prev_tri - 1;   /* 0 <= offset < d */

    /* Coordinates in reverse order: row = d - offset, col = offset + 1. */
    idx_t row = d - offset;
    idx_t col = offset + 1;

    return nth_nondivisor(col, row);
}
