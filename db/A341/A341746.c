/*
 * A341746  If the runs in the binary expansion of n are (r_1, ..., r_k), then
 *          the runs in the binary expansion of a(n) are (r_1 + ... + r_k,
 *          r_1, ..., r_{k-1}).
 * https://oeis.org/A341746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = combines the run‐lengths of the binary expansion of n
 *            into a new binary number as described in the OEIS entry. For n ≥
 *            1: if the runs in binary(n) are (r1,…,rk) then runs in
 *            binary(a(n)) are (r1+…+rk, r1, …, rk‑1)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static inline size_t bitlen_idx(idx_t n) {
    size_t len = 0;
    while (n) { n >>= 1; ++len; }
    return len;
}

seq_t a(idx_t n) {
    /* n is at least 1 (offset 1), but handle 0 defensively. */
    if (n == 0) return 0;

    /* Compute the run lengths of n's binary representation. */
    size_t max_runs = 65;              /* maximum for 64‑bit n */
    idx_t runs[65];
    size_t k = 0;                      /* number of runs found */

    size_t len = bitlen_idx(n);

    int cur_bit = (n >> (len - 1)) & 1ULL;   /* most significant bit (always 1) */
    idx_t cur_run = 1;                       /* we have seen at least one bit */

    for (int i = (int)len - 2; i >= 0; --i) {    /* already counted MSB */
        int bit = (n >> i) & 1ULL;
        if (bit == cur_bit) {
            ++cur_run;
        } else {
            runs[k++] = cur_run;
            cur_bit = 1 - cur_bit;
            cur_run = 1;
        }
    }
    runs[k++] = cur_run;                   /* last run */

    /* Sum of all run lengths is simply the bit length of n. */
    idx_t total_len = (idx_t)len;

    /* Construct a(n) from the new set of runs. */
    seq_t result = 0;
    int next_bit = 1;      /* first run will be ones */

    /* First run: total_len ones */
    for (idx_t i = 0; i < total_len; ++i) {
        result = (result << 1) | 1;
    }
    /* Toggle bit for next run */
    next_bit ^= 1;

    /* Remaining runs: r1, r2, ..., rk-1 */
    for (size_t i = 0; i + 1 < k; ++i) {   /* i from 0 to k-2 */
        idx_t run_len = runs[i];
        if (next_bit) {                    /* ones run */
            for (idx_t j = 0; j < run_len; ++j) {
                result = (result << 1) | 1;
            }
        } else {                           /* zeros run */
            result <<= run_len;
        }
        next_bit ^= 1;                     /* toggle */
    }

    return result;
}
