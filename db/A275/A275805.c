/*
 * A275805  Indices of nonsquarefree terms in A275734; numbers with at least
 *          one digit slope (in their factorial base representation) with
 *          multiple nonzero digits. (See comments for the exact definition).
 * https://oeis.org/A275805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   sequence defined by the set of integers n for which in the
 *            factorial base representation of n there exist two distinct
 *            positions i1,i2 with non‑zero digits d1,d2 such that (i1-d1) ==
 *            (i2-d2). The sequence a(n) lists these numbers in increasing
 *            order, with a(1) = 5, a(2) = 11, etc. The function be...
 * Range:     u8 n <= 140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool qualifies(seq_t num) {
    seq_t t = num;
    int i = 1;                 // factorial position (1‑based)
    int vals[64];
    int vcnt = 0;
    while (t > 0) {
        seq_t d = t % (seq_t)(i + 1);   // digit at position i
        t /= (seq_t)(i + 1);
        if (d != 0) {
            unsigned long long dv = (unsigned long long)d;
            int v = i - (int)dv;        // i - d
            /* Check for duplicate v among earlier non‑zero digits */
            for (int j = 0; j < vcnt; ++j)
                if (vals[j] == v) return true;
            vals[vcnt++] = v;
        }
        ++i;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;           // how many qualifying numbers found
    seq_t candidate = 0;       // current integer being tested
    while (count < n) {
        ++candidate;           // sequence is 1‑indexed, skip 0
        if (qualifies((seq_t)candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
    }
    /* unreachable but required for strict compilers */
    return 0;
}
