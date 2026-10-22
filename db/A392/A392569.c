/*
 * A392569  Partial sums of A000788.
 * https://oeis.org/A392569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k=1..n} (n - k + 1) * popcount(k)
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where popcount(k) is the number of 1‑bits in k's binary representation */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t w = (seq_t)n;               /* weight starts at n and decreases */
    for (idx_t k = 1; k <= n; ++k, --w)
    {
        /* popcount of k using GCC/Clang builtin; cast to unsigned long long */
        idx_t cnt = __builtin_popcountll((unsigned long long)k);
        total += w * (seq_t)cnt;
    }
    return total;
}
