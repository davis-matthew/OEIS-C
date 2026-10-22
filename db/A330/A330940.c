/*
 * A330940  a(n) is the least value whose binary representation can be
 *          obtained by interleaving (or shuffling) two copies of the binary
 *          representation of n.
 * https://oeis.org/A330940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest integer whose binary representation can be
 *            formed by interleaving two copies of the binary representation
 *            of n while preserving the order within each copy
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>


seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Length L of the binary representation of n. */
    idx_t L = 0;
    idx_t t = n;
    while (t) {
        ++L;
        t >>= 1;
    }

    /* b[i] holds the i‑th bit of n (0 = MSB). */
    uint8_t b[64];                    /* L ≤ 64 for idx_t = uint64_t */
    for (idx_t i = 0; i < L; ++i)
        b[i] = (uint8_t)((n >> (L - 1 - i)) & 1);

    /* Dynamic programming table: dp[i][j] is the minimal suffix value
       when i bits of the first copy and j bits of the second copy
       have already been used. */
    seq_t dp[65][65];                 /* L ≤ 64 → 65×65 table */

    dp[L][L] = 0;                     /* empty suffix */

    /* States where one copy is exhausted. */
    for (idx_t j = L; j-- > 0; ) {
        idx_t rem = L - j;            /* remaining bits in the second copy */
        dp[L][j] = ((seq_t)b[j] << (rem - 1)) | dp[L][j + 1];
    }
    for (idx_t i = L; i-- > 0; ) {
        idx_t rem = L - i;            /* remaining bits in the first copy */
        dp[i][L] = ((seq_t)b[i] << (rem - 1)) | dp[i + 1][L];
    }

    /* General states. */
    for (idx_t i = L; i-- > 0; ) {
        for (idx_t j = L; j-- > 0; ) {
            idx_t rem = 2 * L - i - j;   /* bits still to place */
            seq_t cand1 = ((seq_t)b[i] << (rem - 1)) | dp[i + 1][j];
            seq_t cand2 = ((seq_t)b[j] << (rem - 1)) | dp[i][j + 1];
            dp[i][j] = cand1 < cand2 ? cand1 : cand2;
        }
    }

    return dp[0][0];
}
