/*
 * A342758  Array read by ascending antidiagonals: T(k, n) is the maximum
 *          value of the magic constant in a perimeter-magic k-gon of order n.
 * https://oeis.org/A342758
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A342758:  T(k,n) = (n + k*(n^2-2) + ((k mod 2)-1)*(n mod 2))/2
 *  The sequence a(n) lists T(k,n) along ascending antidiagonals, starting
 *  with a(3)=T(3,3).  For n ≥ 3, a(n) is obtained by
 *  – determining the diagonal sum s = k+n,
 *  – finding the offset within that diagonal,
 *  – computing k and n from the offset,
 *  – evaluating T(k,n) with the closed formula above.
 *  This implementation uses only integer arithmetic and works for any
 *  integer types seq_t (unsigned, 8/16/32/64/128 or arbitrary‑precision)
 *  and idx_t (uint64_t).  No external libraries are required.
 */

seq_t a(idx_t n)
{
    /* Terms below the offset 3 are undefined; return 0. */
    if (n < 3) return 0;

    /* Zero‑based index into the sequence starting at a(3). */
    idx_t idx = n - 3;
    idx_t m   = idx + 1;        /* 1‑based position within all terms */

    /* Find smallest x such that x*(x+1)/2 ≥ m.
     * x = s - 5, where s = k + n.  Diagonal length is x. */
    idx_t x = 1;
    while ( ((unsigned __int128)x * (x + 1) / 2) < (unsigned __int128)m )
        ++x;

    /* Number of terms before this diagonal.  C(s-1) = (x-1)*x/2. */
    idx_t before = (x - 1) * x / 2;
    idx_t offset = idx - before;          /* position within the diagonal [0..x-1] */

    /* Reconstruct k and n from the diagonal index and offset. */
    seq_t n_val = (seq_t)3 + (seq_t)offset;          /* 3 + offset */
    seq_t k_val = (seq_t)x + 2 - (seq_t)offset;      /* (x+2) - offset */

    /* Compute T(k,n) = ( n + k*(n^2 - 2) + ((k mod 2)-1)*(n mod 2) ) / 2 */
    seq_t num = n_val + k_val * (n_val * n_val - 2);
    if ( (k_val & 1ULL) == 0 && (n_val & 1ULL) == 1 )
        num -= 1;       /* subtract for k even and n odd */

    return num / 2;
}
