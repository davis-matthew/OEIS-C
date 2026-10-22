/*
 * A303300  Irregular triangle read by rows: T(n,k) is the number of
 *          partitions of n into k consecutive parts that differ by two,
 *          including the partition n, and the first element of column k is in
 *          row k^2.
 * https://oeis.org/A303300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   a(n) is the n-th element of the irregular triangle read by rows.
   For each row r (r ≥ 1) the triangle has kmax = ⌊√r⌋ columns, numbered
   1…kmax.  The value at column k is 1 iff r is divisible by k,
   otherwise 0.  The sequence a(n) is obtained by concatenating the rows
   in order.  Thus for the n-th term we find the row r such that

       Σ_{i=1}^{r-1} ⌊√i⌋ < n ≤ Σ_{i=1}^{r} ⌊√i⌋

   and the column c = n – Σ_{i=1}^{r-1} ⌊√i⌋.  Finally

       a(n) = 1  if  r % c == 0
              0  otherwise
*/
#include <stdint.h>
#include <stdbool.h>

static inline idx_t isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t low = 1, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid <= n / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    idx_t cum = 0;
    idx_t r = 1;
    while (1) {
        idx_t kmax = isqrt_u64(r);
        if (cum + kmax >= n) break;
        cum += kmax;
        ++r;
    }
    idx_t offset = n - cum - 1;   /* 0‑based offset within the row   */
    idx_t c = offset + 1;
    return (seq_t)(r % c == 0);
}
