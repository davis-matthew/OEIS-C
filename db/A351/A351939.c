/*
 * A351939  Irregular triangle read by rows: the n-th row contains the values
 *          0..2^n-1 sorted first by Hamming weight and then by position in
 *          reflected Gray code.
 * https://oeis.org/A351939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 519, u16 n <= 9815, u32 n <= 9815, u64 n <= 9815,
 *            u128 n <= 9815, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A351939: the n‑th element of the triangle read row‑wise.
   Row r is the list of 0..2^r-1 sorted by Hamming weight then by
   reflected Gray code position.  */
#include <stdint.h>
#include <stddef.h>

static inline unsigned long long popcnt64(unsigned long long x)
{
    return __builtin_popcountll(x);
}

seq_t a(idx_t n)
{
    /* Determine which row contains index n.
       R is the largest integer such that 2^R <= n+1.   */
    idx_t np1 = n + 1;
    /* np1 is guaranteed non‑zero because n >= 0. */
    size_t R = 63 - __builtin_clzll((unsigned long long)np1);

    /* Number of elements before this row is 2^R - 1.  */
    idx_t prev_base = (idx_t)1 << R;
    /* Position inside the row (0‑based).  */
    size_t offset = (size_t)(n - prev_base + 1);

    size_t len = (size_t)1 << R;          /* 2^R elements in this row */
    size_t remaining = offset;            /* how many we still need to skip */

    /* Iterate through weights in ascending order; for each weight
       we scan the entire Gray‑code sequence and pick only the
       entries whose weight matches.  */
    for (size_t w = 0; w <= R; ++w) {
        for (size_t i = 0; i < len; ++i) {
            idx_t gray = (idx_t)i ^ ((idx_t)i >> 1);   /* reflected Gray code */
            if (popcnt64((unsigned long long)gray) == w) {
                if (remaining == 0)
                    return (seq_t)gray;                /* found the element */
                --remaining;
            }
        }
    }

    /* The algorithm should always return inside the loops.  */
    return (seq_t)0;
}
