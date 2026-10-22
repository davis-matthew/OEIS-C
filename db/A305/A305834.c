/*
 * A305834  Triangle read by rows: T(0,0)= 1; T(n,k)= T(n-1,k) + 4*T(n-2,k-1)
 *          for k = 0..floor(n/2); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A305834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) = T(r, k) where r is the unique non‑negative integer with
 *            Σ_{i=0}^{r} (⌊i/2⌋ + 1) > n, k = n - Σ_{i=0}^{r-1} (⌊i/2⌋ + 1);
 *            T(r, k) = binom(r - k, k) * 4^k
 * Range:     u8 n <= 18, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_nk(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find the row r that contains the n‑th element */
    idx_t r = 0, sum = 0;
    while (1) {
        idx_t len = r / 2 + 1;             /* number of elements in row r */
        if (sum + len > n) break;
        sum += len;
        ++r;
    }

    idx_t k = n - sum;                      /* position within row r */

    /* compute T(r, k) = binom(r - k, k) * 4^k */
    seq_t bin = binom_nk(r - k, k);

    seq_t pow4 = 1;
    for (idx_t i = 0; i < k; ++i)
        pow4 *= (seq_t)4;

    return bin * pow4;
}
