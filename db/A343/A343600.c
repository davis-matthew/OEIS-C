/*
 * A343600  For any positive number n, the ternary representation of a(n) is
 *          obtained by left-rotating the ternary representation of n until a
 *          nonzero digit appears again as the leftmost digit; a(0) = 0.
 * https://oeis.org/A343600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the ternary rotation of n that brings the next non‑zero
 *            digit to the most significant position; a(0)=0. For any n≥1,
 *            write n in base 3 as d0 d1 … d_{k-1} (d0≠0). Find the smallest
 *            j≥1 such that d_j≠0 (if none, j=k and a(n)=n). Then a(n) = d_j
 *            d_{j+1} … d_{k-1} d_0 d_1 … d_{j-1} (interpret...
 * Range:     u8 n <= 243, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* step 1: convert n to base‑3 digits in most‑significant first order */
    char rev[64];
    int cnt = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        rev[cnt++] = tmp % 3;          /* least‑significant digit first */
        tmp /= 3;
    }
    /* reverse to get most‑significant first */
    char digits[64];
    for (int i = 0; i < cnt; ++i)
        digits[i] = rev[cnt - 1 - i];

    /* step 2: find the first non‑zero digit after the first */
    int j = 1;
    while (j < cnt && digits[j] == 0)
        ++j;

    /* if no such digit, a(n) = n */
    if (j == cnt)
        return (seq_t)n;

    /* step 3: build the rotated value */
    seq_t res = 0;
    for (int i = j; i < cnt; ++i)
        res = res * 3 + digits[i];
    for (int i = 0; i < j; ++i)
        res = res * 3 + digits[i];

    return res;
}
