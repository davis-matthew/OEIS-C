/*
 * A340785  Number of factorizations of 2n into even factors > 1.
 * https://oeis.org/A340785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of 2*n into even
 *            factors > 1
 * Range:     u8 n <= 5759, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 101 terms through n = 9909 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t rec_factorization(idx_t cur, idx_t min_factor) {
    if (cur == 1) {
        return 1;
    }
    seq_t total = 0;
    for (idx_t d = min_factor; d <= cur; ++d) {
        if ((d & 1) != 0) {            /* skip odd divisors */
            continue;
        }
        if (cur % d != 0) {
            continue;                  /* d must divide cur */
        }
        total += rec_factorization(cur / d, d);
    }
    return total;
}

seq_t a(idx_t n) {
    idx_t prod = 2 * n;                /* compute 2*n  */
    return rec_factorization(prod, 2); /* start recursion with factor 2 */
}
