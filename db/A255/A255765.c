/*
 * A255765  Partial sums of A255744.
 * https://oeis.org/A255765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=2..n} 10·9^{popcount(k-1)-1} (with a(1)=1)
 * Range:     u8 n <= 6, u16 n <= 31, u32 n <= 991, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation of sequence A255765: partial sums of A255744.
    A255744(n) = 1          for n = 1
    A255744(n) = 10 * 9^(pc-1)  for n ≥ 2 , pc = popcount(n‑1)   */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* Base case: a(1) = 1 */
    if (n == 0) return (seq_t)0;          /* not part of the offset but harmless */
    seq_t sum = (seq_t)1;
    if (n == 1) return sum;

    /* Pre‑compute powers of 9 up to the maximum popcount that can occur.
       For n ≤ 10^4 the popcount of n–1 is ≤ 14; we safe‑guard with room for 64.   */
    static seq_t pow9[65];
    static bool initialized = false;

    if (!initialized) {
        pow9[0] = (seq_t)1;
        for (int i = 1; i < 65; ++i)
            pow9[i] = pow9[i - 1] * (seq_t)9;
        initialized = true;
    }

    /* Accumulate the differences.  Each term is 10 * 9^(popcount(k‑1)‑1).   */
    for (idx_t k = 2; k <= n; ++k) {
        /* popcount of k-1 (unsigned 64‑bit).   */
        unsigned int pc = __builtin_popcountll(k - 1);
        /* pc ≥ 1 for k ≥ 2, so pc-1 is a valid index.  */
        seq_t diff = (seq_t)10 * pow9[pc - 1];
        sum += diff;
    }
    return sum;
}
