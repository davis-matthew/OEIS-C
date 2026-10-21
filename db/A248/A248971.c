/*
 * A248971  Triangular array read by rows.
 *          T(n,k)=C(k,2)+C(n-k,2),n>=2,1<=k<=floor(n/2).
 * https://oeis.org/A248971
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = C(k,2)+C(n-k,2) (triangular array read by rows)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static inline seq_t binom2(seq_t x)
{
    return (x < 2) ? 0 : (x * (x - 1)) / 2;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* Under the offset – behaviour irrelevant */
    idx_t idx = n - 2;            /* zero‑based position in the flattened array  */
    idx_t r = 2;                  /* current row (n ≥ 2)                          */
    idx_t sum = 0;                /* total terms before this row                   */

    for (;;)
    {
        idx_t len = r >> 1;       /* floor(r/2) – number of terms in the row      */
        if (sum + len > idx)      /* found the row containing the desired term    */
        {
            idx_t k = idx - sum + 1;           /* 1‑based column in the row          */
            seq_t r_seq = (seq_t)r;
            seq_t k_seq = (seq_t)k;
            return binom2(k_seq) + binom2(r_seq - k_seq);
        }
        sum += len;
        r++;
    }
}
