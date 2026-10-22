/*
 * A373749  Triangle read by rows: T(n, k) = MOD(k^2, n) where MOD(a, n) = a
 *          if n = 0 and otherwise a - n*floor(a/n). (Quadratic residue.)
 * https://oeis.org/A373749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = MOD(k², r), where r = ⎣(√(8·n+1) – 1)/2⎦ and k = n –
 *            r(r+1)/2
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    idx_t cumulative = 0;   /* number of elements before the current row */

    /* Incrementally search for r. This loop is O(√n) and trivial for n ≤ 10000. */
    while (1) {
        idx_t next_count = cumulative + (r + 1);
        if (next_count > n)
            break;
        cumulative = next_count;
        ++r;
    }

    /* k is the position inside row r (0‑based). */
    idx_t k = n - cumulative;

    seq_t ksq = (seq_t)k * (seq_t)k;

    /* MOD(a,0) is defined as a; for r = 0 the row has only one element 0. */
    if (r == 0)
        return ksq;

    return ksq % (seq_t)r;
}
