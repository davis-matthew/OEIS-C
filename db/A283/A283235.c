/*
 * A283235  Triangle read by rows: n-th row gives the numbers of primes p such
 *          that p*prime(k) <= prime(n)^2, k=1..n.
 * https://oeis.org/A283235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A283235: Triangle read by rows – numbers of primes p
   such that p*prime(k) ≤ prime(n)², for k = 1..n.
   The sequence is read row‑by‑row; a(n) is the index‑flattened
   entry, where row n has n elements. */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Find the row (i) and column (j) corresponding to index n. */
    idx_t i = 1;
    idx_t cum = 1; /* cumulative number of terms up to current row */

    while (n > cum) {
        ++i;
        cum += i;           /* after the loop cum == i*(i+1)/2 */
    }

    /* Column index (1‑based).  cum_before = cum - i. */
    idx_t j = n - (cum - i);

    /* Retrieve the primes needed for the row and column. */
    uint32_t p_row = get_prime(i);   /* prime(n)  */
    uint32_t p_col = get_prime(j);   /* prime(k)  */

    /* Upper bound for counting primes. */
    seq_t limit = (seq_t)p_row * (seq_t)p_row / (seq_t)p_col;

    /* Return π(limit). */
    return compute_pi((idx_t)limit);
}
