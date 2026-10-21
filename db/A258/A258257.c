/*
 * A258257  The number of representations of n as a minimal number of
 *          triangular numbers, A000217(n).
 * https://oeis.org/A258257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts representations of n as a sum of the minimal number
 *            of triangular numbers T_k = k(k+1)/2. For each n >= 1 – first
 *            check if n itself is triangular (minimal number 1). If not,
 *            count unordered pairs of triangular numbers that sum to n; if
 *            any exist, the minimal number is 2. Otherwise the...
 * Range:     u8 n <= 742, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not in the OEIS range; safe guard */

    /*--------------------- 1) single triangular number ---------------------*/
    for (idx_t i = 1; ; ++i) {
        seq_t t = ((seq_t)i * ((seq_t)i + 1)) / 2;
        if (t == (seq_t)n) return 1;
        if (t > (seq_t)n) break;
    }

    /*--------------------- 2) unordered pairs  ---------------------------*/
    seq_t pairs = 0;
    for (idx_t i = 1; ; ++i) {
        seq_t t1 = ((seq_t)i * ((seq_t)i + 1)) / 2;
        if (t1 > (seq_t)n) break;
        for (idx_t j = i; ; ++j) {
            seq_t t2 = ((seq_t)j * ((seq_t)j + 1)) / 2;
            seq_t s  = t1 + t2;
            if (s == (seq_t)n) {
                ++pairs;
            }
            if (s > (seq_t)n) break;
        }
    }
    if (pairs > 0) return pairs;

    /*--------------------- 3) unordered triples  --------------------------*/
    seq_t triples = 0;
    for (idx_t i = 1; ; ++i) {
        seq_t t1 = ((seq_t)i * ((seq_t)i + 1)) / 2;
        if (t1 > (seq_t)n) break;
        for (idx_t j = i; ; ++j) {
            seq_t t2 = ((seq_t)j * ((seq_t)j + 1)) / 2;
            seq_t sum2 = t1 + t2;
            if (sum2 > (seq_t)n) break;
            for (idx_t k = j; ; ++k) {
                seq_t t3 = ((seq_t)k * ((seq_t)k + 1)) / 2;
                seq_t s = sum2 + t3;
                if (s == (seq_t)n)
                    ++triples;
                if (s > (seq_t)n) break;
            }
        }
    }
    return triples;
}
