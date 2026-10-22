/*
 * A358551  Number of nodes in the ordered rooted tree with binary encoding
 *          A014486(n).
 * https://oeis.org/A358551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that Σ_{i=0}^k C_i ≥ n (C_i = Catalan
 *            number), then return k+1. Here C_0 = 1, C_{m+1} = C_m *
 *            2*(2*m+1)/(m+2)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but return 0 for safety */

    seq_t sum = 1;          /* Σ_{i=0}^{k} C_i, start with C_0 */
    seq_t cat  = 1;          /* current Catalan number C_k, start k=0 */
    idx_t  k    = 0;          /* index of current Catalan number (0‑based) */

    while (sum < (seq_t)n) { /* find smallest k with sum ≥ n */
        /* compute next Catalan number C_{k+1}:
           C_{k+1} = C_k * 2*(2*k+1)/(k+2)  */
        seq_t numerator   = cat * (seq_t)(2 * (2 * k + 1));
        seq_t denominator = (seq_t)(k + 2);
        cat = numerator / denominator;   /* exact division */

        k++;          /* move to next index */
        sum += cat;   /* add new Catalan number to cumulative sum */
    }
    return (seq_t)(k + 1);   /* convert 0‑based k to 1‑based a(n) */
}
