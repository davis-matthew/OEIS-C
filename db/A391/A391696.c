/*
 * A391696  Irregular triangular array read by rows: T(n,k) = position of 1st
 *          letter of 1st appearance of the k-th word of length n in the
 *          lexicographic ordering of all 01-words, as in A076478.
 * https://oeis.org/A391696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   * Let r = floor(log2(n)) + 1 (the row containing n); * let
 *            a(2^k) be defined by a(1)=1 and * a(2^k) = a(2^{k-1}) +
 *            (k-1)*2^{k-2} + 1 (k >= 1). * Then a(n) = a(2^{r-1}) + (n -
 *            2^{r-1})*(r-1)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* precomputed start values a(2^{r-1}) for r = 1 .. 64 */
    static seq_t starts[65];
    static bool init = false;
    if (!init) {
        starts[1] = 1;                      /* a(1) */
        for (idx_t i = 2; i < 65; ++i) {    /* i = r */
            /* term = (i-2)*2^{i-2} + 1 */
            idx_t pow = (idx_t)1 << (i - 2);
            seq_t term = ((seq_t)(i - 2)) * ((seq_t)pow) + 1;
            starts[i] = starts[i - 1] + term;
        }
        init = true;
    }

    /* find row r: floor(log2(n)) + 1 */
    idx_t r = 1;
    idx_t tmp = n;
    while (tmp >>= 1) {
        ++r;
    }

    /* 2^{r-1} */
    idx_t high = (idx_t)1 << (r - 1);
    idx_t offset = n - high;                 /* 0-based offset inside the row */

    seq_t val = starts[r] + ((seq_t)offset) * ((seq_t)(r - 1));
    return val;
}
