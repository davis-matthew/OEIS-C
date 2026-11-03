/*
 * A356995  a(n) = b(n) - b(b(n)) - b(n - b(n)) for n >= 3, where b(n) =
 *          A356988(n).
 * https://oeis.org/A356995
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(n) – b(b(n)) – b(n – b(n)) (equivalently defined by
 *            block and zero patterns described in the OEIS entry)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 89 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A356995 */
#include <stdbool.h>

static void init_fib_lucas(idx_t maxk, idx_t *fib, idx_t *lucas) {
    fib[0] = 0;
    fib[1] = 1;
    for (idx_t i = 2; i < maxk; ++i) fib[i] = fib[i-1] + fib[i-2];

    lucas[0] = 2;
    lucas[1] = 1;
    for (idx_t i = 2; i < maxk; ++i) lucas[i] = lucas[i-1] + lucas[i-2];
}

seq_t a(idx_t n) {
    /* offset: a(3) is first term */
    if (n <= 6) return (seq_t)0;

    /* Precompute Fibonacci and Lucas up to a safe limit */
    static bool initialized = false;
    static idx_t fib[50], lucas[50];
    if (!initialized) {
        init_fib_lucas(50, fib, lucas);
        initialized = true;
    }

    /* iterate over blocks k >= 4 (L(4)=7) */
    for (idx_t k = 4; k < 50; ++k) {
        idx_t L = lucas[k];
        /* F(k-3) and F(k-2) are needed */
        idx_t F3 = fib[k-3];
        idx_t F2 = fib[k-2];

        idx_t block_start  = L - F3;
        idx_t block_end    = L + F3;
        /* zero length = 2*F(k-2) - 1 */
        idx_t zero_len     = 2 * F2 - 1;
        idx_t zone_end     = block_end + zero_len;

        if (n > zone_end) {
            continue;  /* not in this k's block or trailing zeros */
        }

        /* n is in this zone */
        if (n >= block_start && n <= block_end) {
            /* within the block: a = F3 - |n - L| */
            idx_t diff = (n > L) ? (n - L) : (L - n);
            return (seq_t)(F3 - diff);
        } else {
            /* within zero string */
            return (seq_t)0;
        }
    }

    /* If n is beyond all precomputed zones (shouldn't happen for typical n) */
    return (seq_t)0;
}
