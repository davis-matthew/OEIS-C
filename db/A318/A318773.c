/*
 * A318773  Triangle T(n,k) = 3*T(n-1,k) + T(n-4,k-1) for k = 0..floor(n/4),
 *          with T(0,0) = 1 and T(n,k) = 0 for n or k < 0, read by rows.
 * https://oeis.org/A318773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) is the entry of triangle T(n,k)=3^(n-4k)*binomial(n-3k,k)
 *            read row‑by‑row (row major). The row index r is the smallest
 *            integer with sum_{i=0}^{r-1} (floor(i/4)+1) ≤ n < sum_{i=0}^{r}
 *            (floor(i/4)+1); the column index is c = n - sum_{i=0}^{r-1}
 *            (floor(i/4)+1). Then a(n)=T(r,c)
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 65, u64 n <= 230, u128 n <= 860,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Find the row r and column c for the flattened index n. */
    idx_t r = 0;
    idx_t cum = 0;                   /* number of entries before row r */
    while (1) {
        idx_t len = r / 4 + 1;       /* number of entries in row r */
        if (cum + len > n)
            break;
        cum += len;
        r++;
    }
    idx_t c = n - cum;               /* column within the row */

    /* Compute T(r,c) = 3^(r-4c) * binomial(r-3c, c). */
    idx_t exp = r - 4 * c;
    seq_t bin = binomial(r - 3 * c, c);
    seq_t pp  = pow3(exp);
    return pp * bin;
}
