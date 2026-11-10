/*
 * A286066  Positions of 1 in A286064; complement of A286065.
 * https://oeis.org/A286066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A286064, where A286064 is the
 *            Thue-Morse word with each 0110 replaced by 1 (scanned left to
 *            right). Computed by direct scan
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9883 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int A286066_thue(uint64_t i) {
    return __builtin_parityll((unsigned long long)i);
}

seq_t a(idx_t n) {
    uint64_t pos = 0;      /* position in the Thue-Morse word */
    uint64_t out = 0;      /* 1-based index in the transformed word */
    uint64_t ones = 0;     /* number of 1s seen so far */
    while (1) {
        int v;
        if (A286066_thue(pos) == 0 && A286066_thue(pos + 1) == 1 &&
            A286066_thue(pos + 2) == 1 && A286066_thue(pos + 3) == 0) {
            v = 1;
            pos += 4;
        } else {
            v = A286066_thue(pos);
            pos++;
        }
        out++;
        if (v) {
            ones++;
            if (ones == (uint64_t)n) return (seq_t)out;
        }
    }
}
