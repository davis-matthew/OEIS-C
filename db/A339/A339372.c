/*
 * A339372  Number of partitions of n into an odd number of Fibonacci parts
 *          (with a single type of 1).
 * https://oeis.org/A339372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into an odd number of Fibonacci
 *            * parts (parts are Fibonacci numbers ≥1, with only a single type
 *            of part equal * to 1). This is equivalent to the OEIS definition
 *            of A339372. The * algorithm counts the partitions with even/odd
 *            part‐counts using an * “unlimited coin...
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Computes a(n) */
seq_t a(idx_t n)
{
    /* Generate Fibonacci parts (1, 2, 3, 5, 8, ...) up to n */
    idx_t *parts = NULL;
    size_t parts_cap = 8, parts_len = 0;
    parts = (idx_t *)malloc(parts_cap * sizeof(idx_t));
    if (!parts) return 0;                 /* allocation failure fallback */

    /* first Fibonacci part is 1 */
    if (parts_len == parts_cap) {
        parts_cap <<= 1;
        parts = (idx_t *)realloc(parts, parts_cap * sizeof(idx_t));
    }
    parts[parts_len++] = 1;

    /* generate subsequent Fibonacci numbers (2, 3, 5, ...) */
    idx_t f1 = 1, f2 = 2;
    if (f2 <= n && parts_len == parts_cap) {
        parts_cap <<= 1;
        parts = (idx_t *)realloc(parts, parts_cap * sizeof(idx_t));
    }
    if (f2 <= n) parts[parts_len++] = f2;

    while (1) {
        idx_t next = f1 + f2;
        if (next > n) break;
        if (parts_len == parts_cap) {
            parts_cap <<= 1;
            parts = (idx_t *)realloc(parts, parts_cap * sizeof(idx_t));
        }
        parts[parts_len++] = next;
        f1 = f2;
        f2 = next;
    }

    /* Allocate DP arrays for even/odd part counts */
    seq_t *even = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *odd  = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!even || !odd) {
        free(parts);
        free(even);
        free(odd);
        return 0;                          /* allocation failure fallback */
    }

    even[0] = 1;                            /* empty partition has even count */

    /* Unlimited coin DP: for each Fibonacci part, update even/odd counts */
    for (size_t pi = 0; pi < parts_len; ++pi) {
        idx_t coin = parts[pi];
        for (idx_t amt = coin; amt <= n; ++amt) {
            seq_t e_before = even[amt - coin];
            seq_t o_before = odd[amt - coin];
            even[amt] += o_before;      /* adding a coin flips parity */
            odd[amt]  += e_before;
        }
    }

    seq_t result = odd[n];                 /* odd part-count partitions */

    /* Clean up */
    free(even);
    free(odd);
    free(parts);

    return result;
}
