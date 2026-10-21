/*
 * A271703  Triangle read by rows: the unsigned Lah numbers T(n, k) =
 *          binomial(n-1, k-1)*n!/k! if n > 0 and k > 0, T(n, 0) = 0^n and
 *          otherwise 0, for n >= 0 and 0 <= k <= n.
 * https://oeis.org/A271703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is obtained by flattening the triangle of unsigned Lah
 *            numbers T(r,c). For an index n (0‑based) let r =
 *            floor((sqrt(8*n+1)-1)/2) c = n - r*(r+1)/2 Then a(n) = T(r,c) =
 *            binomial(r-1,c-1) * prod_{j=c+1}^{r} j (with the convention
 *            T(0,0)=1 and T(r,0)=0 for r>0)
 * Range:     u8 n <= 21, u16 n <= 37, u32 n <= 80, u64 n <= 211,
 *            u128 n <= 563, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"     /* provides seq_t binomial(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0)=1 */
        return (seq_t)1;

    /* binary search for largest r with r*(r+1)/2 <= n */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t before = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - before;

    if (c == 0)          /* T(r,0)=0 for r>0 */
        return (seq_t)0;

    /* compute T(r,c) = binomial(r-1,c-1) * r! / c! */
    seq_t result = binomial(r - 1, c - 1);
    for (idx_t j = c + 1; j <= r; ++j)
        result *= (seq_t)j;

    return result;
}
