/*
 * A256975  G.f.: (1 + x^4 + x^5 + x^6 + x^10 + x^11 + x^12 +
 *          x^16)/Product_{i=1..8} (1 - x^i).
 * https://oeis.org/A256975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k∈{0,4,5,6,10,11,12,16}} p8(n-k), where p8(m) is the
 *            number of partitions of m using parts 1..8. p8 is computed by
 *            the standard coin‑change DP for the 8 parts
 * Range:     u8 n <= 14, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static storage for the partition table p8(m)   */
static seq_t *p8_table   = NULL;   /* array of p8 values              */
static idx_t  p8_size    = 0;      /* number of slots currently alive */

/* Recompute the partition table up to and including index n */
static void recompute_p8(idx_t n)
{
    /* Allocate a new array of size (n+1) elements */
    seq_t *new_table = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!new_table) abort();

    /* Standard 1D DP (coin‑change) for parts 1..8 */
    new_table[0] = 1;                       /* empty partition */

    for (idx_t part = 1; part <= 8; ++part) {
        for (idx_t i = part; i <= n; ++i) {
            new_table[i] += new_table[i - part];
        }
    }

    /* Replace old table if any */
    if (p8_table) free(p8_table);
    p8_table = new_table;
    p8_size  = n + 1;                       /* indices 0 .. n */
}

/* Ensure that p8_table has values up to index n */
/* If the table already covers n, do nothing. */
static void ensure_p8(idx_t n)
{
    if (p8_table && p8_size > n) return;   /* already computed */

    recompute_p8(n);
}

/* Public function that returns the nth term of A256975 */
seq_t a(idx_t n)
{
    ensure_p8(n);                    /* guarantee table up to n */

    /* Shifts from the numerator polynomial */
    static const idx_t shifts[] = {0, 4, 5, 6, 10, 11, 12, 16};
    const size_t num_shifts = sizeof(shifts) / sizeof(shifts[0]);

    seq_t result = 0;
    for (size_t i = 0; i < num_shifts; ++i) {
        idx_t k = shifts[i];
        if (n >= k) result += p8_table[n - k];
    }
    return result;
}
