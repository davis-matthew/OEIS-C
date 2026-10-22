/*
 * A365089  The Thue-Morse sequence along the sequence of cubes.
 * https://oeis.org/A365089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A010060(n^3) (Thue–Morse value of n^3)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^3 using 192‑bit arithmetic split into three 64‑bit limbs. */
    __uint128_t n128 = ( __uint128_t)n * n;          /* n²  → 128 bits   */
    uint64_t n2_lo = (uint64_t)n128;                /* low 64 bits of n² */
    uint64_t n2_hi = (uint64_t)(n128 >> 64);         /* high 64 bits of n² */

    /* Multiply n² by n : 128 × 64 → 192 bits. */
    __uint128_t prod_low = ( __uint128_t)n * n2_lo;  /* lower 128 bits of product */
    uint64_t prod_low_lo = (uint64_t)prod_low;
    uint64_t prod_low_hi = (uint64_t)(prod_low >> 64);

    __uint128_t prod_mid = ( __uint128_t)n * n2_hi;  /* contributes to bits 64‑127 */
    uint64_t prod_mid_lo = (uint64_t)prod_mid;       /* lower 64 bits of high part */
    uint64_t prod_mid_hi = (uint64_t)(prod_mid >> 64);/* upper 64 bits of high part */

    /* Combine the three 64-bit limbs, taking care of carries. */
    uint64_t limb0 = prod_low_lo;                    /* bits 0‑63  */
    uint64_t limb1 = prod_low_hi + prod_mid_lo;      /* bits 64‑127 */
    uint64_t carry = (limb1 < prod_low_hi) ? 1ULL : 0ULL;
    uint64_t limb2 = prod_mid_hi + carry;            /* bits 128‑191 */

    /* Parity of popcount is XOR of parity of each limb. */
    int parity = __builtin_parityll(limb0) ^ __builtin_parityll(limb1) ^ __builtin_parityll(limb2);

    return (seq_t)parity;
}
