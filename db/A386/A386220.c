/*
 * A386220  a(n) = d + 2*2^e where n = d*2^e with d = A000265(n) odd.
 * https://oeis.org/A386220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A000265(n) + A171977(n) a(n) = odd_part(n) + 2*2^{v2(n)},
 *            where v2(n) is the exponent of 2 in n (the largest e ≥ 0 such
 *            that 2^e | n). Indexing starts at 1
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. */
    idx_t d = n;   /* odd part (A000265) */
    idx_t e = 0;   /* exponent of 2 (A007814) */

    /* Count trailing zeros to find e and d. */
    while ((d & 1ULL) == 0) {
        d >>= 1;
        e++;
    }

    /* Compute 2^(e+1).  Perform shift in the 64‑bit idx_t type to avoid
       UB from shifting a smaller seq_t type. */
    idx_t pow2 = ((idx_t)1) << (e + 1);

    /* Return the sum in seq_t.  Casting to seq_t may truncate when
       the value does not fit, but that is acceptable: values that
       exceed the bit width of seq_t are not required to be exact. */
    return (seq_t)d + (seq_t)pow2;
}
