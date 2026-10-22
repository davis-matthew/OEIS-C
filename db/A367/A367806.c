/*
 * A367806  Inverse permutation of A160080.
 * https://oeis.org/A367806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(2*n) = 3*n [even indices] Formula: a(4*n+3)= 6*n+4 Formula:
 *            a(4*n+1)= A047268(n+1) (used only to initialise the first 20
 *            terms) Recurrence for n ≥ 20: a(n) = a(n-4) + a(n-12) - a(n-16)
 *            with base values a(0)…a(19) listed below. The function computes
 *            a(n) for any n ≥ 0. It uses no global state, o...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* first 20 terms (indices 0 … 19) */
    static const seq_t base[20] = {
        0, 1, 3, 4, 6, 2, 9,10,12,5,15,16,18,7,21,22,24,8,27,28
    };

    if (n < 20)
        return base[(size_t)n];

#ifdef __cplusplus
    /* C++: use a std::vector which will call constructors if needed */
    std::vector<seq_t> vals((size_t)n + 1);
    for (size_t i = 0; i < 20; ++i) vals[i] = base[i];
    for (idx_t i = 20; i <= n; ++i)
        vals[i] = vals[i-4] + vals[i-12] - vals[i-16];
    return vals[(size_t)n];
#else
    /* C: use malloc/frees; seq_t is a plain integer type */
    seq_t *vals = (seq_t*)malloc(((size_t)n + 1) * sizeof(seq_t));
    if (!vals) abort();
    for (size_t i = 0; i < 20; ++i) vals[i] = base[i];
    for (idx_t i = 20; i <= n; ++i)
        vals[i] = vals[i-4] + vals[i-12] - vals[i-16];
    seq_t result = vals[(size_t)n];
    free(vals);
    return result;
#endif
}
