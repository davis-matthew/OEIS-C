/*
 * A352449  2^k appears in the binary expansion of a(n) iff 2^k appears in the
 *          binary expansion of n and k AND n = k (where AND denotes the
 *          bitwise AND operator).
 * https://oeis.org/A352449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 263, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = Σ 2^k  over all bit positions k of n with (k & n) == k */
seq_t a(idx_t n)
{
    /* The result fits in the same number of bits as n (≤64 bits). */
    uint64_t res = 0;
    for (uint64_t k = 0; k < 64; ++k) {
        if ((n >> k) & 1ULL) {                /* bit k is set in n */
            if ((k & n) == k) {               /* condition on k and n */
                res |= 1ULL << k;             /* set bit k in a(n) */
            }
        }
    }
    return (seq_t)res;
}
