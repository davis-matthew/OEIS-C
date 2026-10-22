/*
 * A371053  Ternary numbers consisting of a run of 2's, then a run of 1's,
 *          then a run of 0's.
 * https://oeis.org/A371053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate ternary numbers consisting of a run of 2's, then a run of 1's,
   then a run of 0's, each run being at least one digit, sorted by numeric
   value.  The nth term (n≥1) is returned as a seq_t. */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;           /* undefined for n<1, return 0 */

    /* Find the minimal length L with C(L,3) ≥ n. */
    idx_t L = 3;
    while (1) {
        unsigned __int128 comb = (unsigned __int128)L * (L - 1) * (L - 2) / 6;
        if (comb >= n) break;
        ++L;
    }

    /* Count of terms with length < L.  C(L-1,3). */
    unsigned __int128 comb_prev = (unsigned __int128)(L - 1) * (L - 2) * (L - 3) / 6;
    idx_t prev = (idx_t)comb_prev;          /* safe: comb_prev ≤ n */

    idx_t offset = n - prev - 1;            /* 0‑based index within length L */

    /* Enumerate all triples (a,b,c) with a,b,c≥1 and a+b+c=L
       in lexicographic order, stop at the desired offset. */
    idx_t idx = 0;
    for (idx_t a_len = 1; a_len <= L - 2; ++a_len) {
        for (idx_t b_len = 1; b_len <= L - a_len - 1; ++b_len) {
            if (idx == offset) {
                idx_t c_len = L - a_len - b_len;
                seq_t val = 0;
                for (idx_t i = 0; i < a_len; ++i) val = val * 10 + 2;
                for (idx_t i = 0; i < b_len; ++i) val = val * 10 + 1;
                for (idx_t i = 0; i < c_len; ++i) val = val * 10;   /* +0 */
                return val;
            }
            ++idx;
        }
    }

    return 0;   /* should never reach here */
}
