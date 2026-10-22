/*
 * A329515  Array read by upward antidiagonals: row n = coordination sequence
 *          for cylinder formed by rolling up a strip of width 2*n+1 hexagons
 *          cut from the hexagonal grid by cuts perpendicular to grid lines.
 * https://oeis.org/A329515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is obtained by reading upward antidiagonals of an infinite
 *            array where row r (width 2*r+1) contains the coordination
 *            sequence for a hexagonal‑grid cylinder of that width. For each
 *            row r: r = 0 : 1, 2, 2, 2, … r ≥ 1 : arr[r][0] = 1 arr[r][1] = 3
 *            for 2 ≤ k ≤ 2*r : arr[r][k] = 3*k arr[r][2*r+1]...
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find the diagonal index s so that T(s-1) ≤ n < T(s),
       where T(s) = (s+1)(s+2)/2.  We build T(s) incrementally to avoid
       overflow. */
    idx_t s = 0;
    unsigned __int128 t = 1;          /* T(0) */
    while (t <= n) {
        ++s;
        t += (unsigned __int128)(s + 1);   /* T(s) = T(s‑1) + (s+1) */
    }

    /* T(s‑1) = T(s) – (s+1) */
    unsigned __int128 prev128 = t - (unsigned __int128)(s + 1);
    idx_t prev = (idx_t)prev128;     /* number of terms before diagonal s */

    idx_t p   = n - prev;            /* position within diagonal s (0‑based) */
    idx_t m   = p;                   /* column index */
    idx_t row = s - p;               /* row index */

    if (row == 0) {
        return (m == 0) ? (seq_t)1 : (seq_t)2;
    }

    if (m == 0) return (seq_t)1;
    if (m == 1) return (seq_t)3;
    if (m <= 2 * row) return (seq_t)(3 * m);
    if (m == 2 * row + 1) return (seq_t)(5 * row + 2);
    return (seq_t)(4 * row + 2);
}
