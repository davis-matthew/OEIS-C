/*
 * A261188  Integers such that no subsequence of decimal representation is
 *          divisible by 3.
 * https://oeis.org/A261188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A261188: integers such that no subsequence of decimal
   representation is divisible by 3.  The sequence consists of the
   following numbers in increasing order:

   - all 1‑digit numbers whose digit ≠ 0 mod 3  → 1,2,4,5,7,8
   - all 2‑digit numbers whose two digits are
     either both ≡ 1 mod 3 or both ≡ 2 mod 3 → 11,14,17,22,25,28,
     41,44,47,52,55,58,71,74,77,82,85,88

   No longer numbers can satisfy the condition because the partial
   sums modulo 3 would then repeat.  This function returns the n‑th
   term (with n ≥ 1).  For n beyond the known terms it returns 0. */

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;                 /* undefined below offset */

    /* 1‑digit terms */
    const seq_t single[6] =
    { (seq_t)1, (seq_t)2, (seq_t)4,
      (seq_t)5, (seq_t)7, (seq_t)8 };

    if (n <= (idx_t)6)
        return single[n - (idx_t)1];

    /* 2‑digit terms */
    const seq_t first[6] =
    { (seq_t)1, (seq_t)2, (seq_t)4,
      (seq_t)5, (seq_t)7, (seq_t)8 };
    const seq_t sec1[3] = { (seq_t)1, (seq_t)4, (seq_t)7 }; /* residue 1 */
    const seq_t sec2[3] = { (seq_t)2, (seq_t)5, (seq_t)8 }; /* residue 2 */

    idx_t k = n - (idx_t)7;                /* zero‑based index in 2‑digit list */
    if (k >= (idx_t)18)
        return (seq_t)0;                   /* out of known range */

    seq_t f = first[k / (idx_t)3];         /* first digit of the number   */
    seq_t r = f % (seq_t)3;                /* residue of the digit (1 or 2) */

    seq_t s = (r == (seq_t)1) ? sec1[k % (idx_t)3]
                              : sec2[k % (idx_t)3];

    return f * (seq_t)10 + s;
}
