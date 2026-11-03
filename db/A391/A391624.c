/*
 * A391624  Positive integers k such that there is at most one nonnegative
 *          sequence whose first sums are the k-th composition in standard
 *          order.
 * https://oeis.org/A391624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391624: Positive integers k such that there is at most one
 * nonnegative sequence whose first sums are the k‑th composition in standard order.
 * The test for a given k uses the following criterion:
 *   Let s be the composition of k (as described in the OEIS entry).
 *   Define S_k recursively by S_1 = 0 and
 *          S_k = -S_{k-1} + s_{k-1}   for k ≥ 2  (s_{i} is the i‑th part of s).
 *   Let L = max ( -S_k ) over odd k,      U = min ( S_k ) over even k.
 *   Then there are at most one integer b = y_1 such that the derived sequence
 *   y satisfies y_j + y_{j+1} = s_j (j = 1 … |s|).  Thus k qualifies iff
 *   U ≤ L (zero or exactly one solution). */

#include <stdint.h>
#include <stdbool.h>

static bool _qualifies(uint64_t k) {
    /* Find positions of 1 bits of k (LSB = position 1) */
    int pos[128];
    int pc = 0;
    uint64_t t = k;
    int bit = 1;
    while (t) {
        if (t & 1ULL) pos[pc++] = bit;
        t >>= 1;
        bit++;
    }
    if (pc < 1) return false;            /* composition length 0 → never qualifies */

    /* Compute parts (first differences) of the composition */
    int parts[128];
    for (int i = 0; i < pc; ++i) {
        int prev = (i == 0) ? 0 : pos[i-1];
        parts[i] = pos[i] - prev;
    }

    /* Apply the test on the reversed parts (the composition itself) */
    __int128 S = 0;                     /* current S_k */
    __int128 L = 0;                     /* lower bound, k=1 is odd */
    __int128 U = (__int128)1 << 120;    /* upper bound sentinel */

    /* k = 1 odd: L = max(L, -S) → L = 0 already */
    for (int curr = 2; curr <= pc + 1; ++curr) {
        int idx = pc - curr + 1;         /* index in 'parts' for s_{curr-1} */
        int s_val = parts[idx];
        S = -S + (__int128)s_val;       /* update S */
        if (curr & 1) {                 /* odd k */
            __int128 bound = -S;
            if (bound > L) L = bound;
        } else {                        /* even k */
            if (S < U) U = S;
        }
    }

    /* At most one solution iff U <= L                                       */
    return (U <= L);
}

#define CACHE_CAP 20000   /* sufficient for n up to about 10000 */

seq_t a(idx_t n) {
    static seq_t cache[CACHE_CAP];
    static idx_t cache_size = 0;
    static uint64_t next_k = 1;

    while (cache_size < n) {
        if (_qualifies(next_k)) {
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
