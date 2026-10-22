/*
 * A374993  Total cost when the elements of the n-th composition (in standard
 *          order) are requested from a self-organizing list initialized to
 *          (1, 2, 3, ...), using the transpose updating strategy.
 * https://oeis.org/A374993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Total cost when the elements of the n‑th composition (in standard order)
   are requested from a self‑organizing list initialized to (1,2,3,…),
   using the transpose updating strategy.  The function generates the
   n‑th composition (via binary representation of its index) and
   simulates the list to compute the exact total cost. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Find m such that 2^(m‑1) ≤ n < 2^m  (m ≥ 1) */
    idx_t m = 0;
    while (((idx_t)1 << m) <= n) {
        ++m;
    }

    /* Index of the composition within all compositions of m (0‑based). */
    idx_t idx = n - ((idx_t)1 << (m - 1));

    /* Generate the composition parts. */
    int parts[64];
    int parts_n = 0;
    int cur     = 1;

    if (m == 1) {
        parts[parts_n++] = 1;
    } else {
        /* There are m‑1 bits in the binary representation of idx. */
        for (int i = (int)(m - 2); i >= 0; --i) {
            if ((idx >> i) & 1ULL) {
                parts[parts_n++] = cur;
                cur = 1;
            } else {
                ++cur;
            }
        }
        parts[parts_n++] = cur;
    }

    /* Simulate the transpose‑on‑request list. */
    int list[128];
    for (int i = 0; i < (int)m; ++i) list[i] = i + 1;

    seq_t cost = 0;
    for (int pi = 0; pi < parts_n; ++pi) {
        int request = parts[pi];
        int pos = 0;
        while (list[pos] != request) ++pos;          /* 0‑based position */
        cost += (seq_t)(pos + 1);                   /* cost = 1‑based index */
        if (pos > 0) {                               /* transpose swap */
            int tmp = list[pos];
            list[pos] = list[pos - 1];
            list[pos - 1] = tmp;
        }
    }
    return cost;
}
