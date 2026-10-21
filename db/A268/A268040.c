/*
 * A268040  Array y AND NOT x, read by antidiagonals.
 * https://oeis.org/A268040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) = i & ~j, where (i, j) are the Cantor pair coordinates of n
 *            read along antidiagonals. i = n - s*(s+1)/2 (offset along the
 *            s‑th diagonal) j = s - i, s = ⌊(−1 + √(8n + 1)) / 2⌋ Since ~j is
 *            an infinite‑width NOT, i & ~j = i - (i & j)
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal index s such that
       s*(s+1)/2 ≤ n < (s+1)*(s+2)/2.  */
    idx_t s = 0;
    while ((__uint128_t)(s + 1) * (s + 2) / 2 <= n)
        ++s;

    /* Number of elements before diagonal s */
    __uint128_t prod = (__uint128_t)s * (s + 1);
    idx_t prev = (idx_t)(prod / 2);

    /* Position within diagonal s */
    idx_t d = n - prev;          /* 0 ≤ d ≤ s */
    idx_t i = d;
    idx_t j = s - d;

    /* Compute i & ~j = i - (i & j) to avoid infinite‑width NOT. */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    return ii - (ii & jj);
}
