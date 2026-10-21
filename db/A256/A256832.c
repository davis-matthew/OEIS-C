/*
 * A256832  Product of first n Pell numbers Pell(1), ... , Pell(n).
 * https://oeis.org/A256832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} Pell(k) where Pell(0)=0, Pell(1)=1,
 *            Pell(k)=2*Pell(k-1)+Pell(k-2)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;   /* offset starts at 1, but handle 0 gracefully */

    seq_t prod = 1;
    seq_t prev = 0;  /* Pell(0) */
    seq_t curr = 1;  /* Pell(1) */

    for (idx_t i = 1; i <= n; ++i) {
        prod *= curr;                   /* multiply by Pell(i) */

        /* compute next Pell number */
        seq_t next = 2 * curr + prev;   /* Pell(i+1) */
        prev = curr;
        curr = next;
    }
    return prod;
}
