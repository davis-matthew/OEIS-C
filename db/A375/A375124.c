/*
 * A375124  Weakly decreasing run-leader transformation for standard
 *          compositions.
 * https://oeis.org/A375124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the leaders of the weakly decreasing runs in the
 *            n‑th composition in graded reverse‑lexicographic order
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the empty composition (n = 0). */
    if (n == 0)
        return 0;

    /* Convert the index to binary and find the sum s of the composition. */
    uint64_t kn = (uint64_t)n;
    /* s = floor(log2(n)) + 1  for n > 0 */
    int s = 64 - __builtin_clzll(kn);

    /* offset of the composition among all compositions of sum s */
    uint64_t offset = kn - (1ULL << (s - 1));

    /* Decode the parts of the composition from the offset. */
    int parts[64];
    int len = 0;
    int partLen = 1;                      /* length of the current part   */
    for (int pos = 1; pos <= s - 1; ++pos) {
        bool separator = ((offset >> (s - 1 - pos)) & 1ULL);
        if (separator) {
            parts[len++] = partLen;
            partLen = 1;
        } else {
            ++partLen;
        }
    }
    parts[len++] = partLen;                /* final part */

    /* Compute the leaders of weakly decreasing runs. */
    int leaders[64];
    int leadlen = 0;
    for (int i = 0; i < len; ++i) {
        if (i == 0 || parts[i - 1] < parts[i])
            leaders[leadlen++] = parts[i];
    }

    /* Sum of the leader parts gives the sum of the leaders composition. */
    int sumLead = 0;
    for (int i = 0; i < leadlen; ++i) sumLead += leaders[i];

    /* Find the index of the leaders composition in standard order. */
    seq_t result;
    if (sumLead == 0)          /* empty leaders composition */
        result = 0;
    else {
        /* Compute the offset of the leaders composition. */
        uint64_t offsetLead = 0;
        int cumulative = 0;
        for (int j = 1; j < leadlen; ++j) {
            cumulative += leaders[j - 1];
            int slot_index = cumulative;                 /* 1 .. sumLead-1 */
            int bit_pos = sumLead - 1 - slot_index;      /* LSB = slot sumLead-1 */
            offsetLead |= (1ULL << bit_pos);
        }
        seq_t base = (seq_t)1;
        base <<= (sumLead - 1);                         /* 2^{sumLead-1} */
        result = base + (seq_t)offsetLead;
    }
    return result;
}
