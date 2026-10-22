/*
 * A385540  Number of values of nonnegative s < n such that s^s == (-s)^s == s
 *          (mod n).
 * https://oeis.org/A385540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ 0 ≤ s < n : s^s ≡ s (mod n) and (-s)^s ≡ s (mod n) }|
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function brute‑forces all s < n, using mod_pow for modular exponentiation. */
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t s = 0; s < n; ++s) {
        seq_t s_seq = (seq_t)s;
        seq_t a_val;
        if (s == 0) {
            /* 0^0 is taken as 1, consistent with OEIS convention */
            a_val = 1 % (seq_t)n;
        } else {
            a_val = mod_pow(s_seq, (uint64_t)s, (seq_t)n);
        }
        if (a_val != s_seq)
            continue;
        seq_t base = ((seq_t)n - s_seq) % (seq_t)n; /* (-s) mod n */
        seq_t b_val = mod_pow(base, (uint64_t)s, (seq_t)n);
        if (b_val == s_seq)
            ++cnt;
    }
    return cnt;
}
