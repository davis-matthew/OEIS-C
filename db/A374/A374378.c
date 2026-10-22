/*
 * A374378  Iterated rascal triangle R2: T(n,k) = Sum_{m=0..2}
 *          binomial(n-k,m)*binomial(k,m).
 * https://oeis.org/A374378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where row and col give the position of the *
 *            entry obtained by scanning the iterated rascal triangle R2
 *            row‑wise. * T(n,k) = Sum_{m=0..2} binomial(n-k,m)*binomial(k,m)
 * Range:     u8 n <= 83, u16 n <= 1147, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the found row. */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;          /* 0 <= k <= r */

    /* Compute T(r,k) using the closed form. */
    seq_t nn = (seq_t)r;
    seq_t kk = (seq_t)k;
    seq_t nk = nn - kk;

    seq_t res = 1;                 /* m = 0 term */
    res += kk * nk;                /* m = 1 term */

    /* m = 2 term, only needed if both k and n-k are at least 2. */
    if (kk > 1 && nk > 1) {
        seq_t part1 = kk * (kk - 1);       /* k*(k-1)    */
        seq_t part2 = nk * (nk - 1);       /* (n-k)*(n-k-1) */
        res += (part1 * part2) / 4;
    }

    return res;
}
