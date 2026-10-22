/*
 * A309517  Sum of the odd parts in the partitions of n into 4 parts.
 * https://oeis.org/A309517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of the odd parts in the partitions of n into 4 parts
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) {
        return 0;
    }

    seq_t total = 0;

    for (idx_t k = 1; k <= n / 4; ++k) {
        idx_t maxJ = (n - k) / 3;
        for (idx_t j = k; j <= maxJ; ++j) {
            idx_t C = n - k - j;                  /* Remaining sum after k and j   */
            idx_t iMin = j;
            idx_t iMax = C / 2;                   /* i ranges from j to floor(C/2) */
            if (iMax < iMin) continue;            /* No valid i for this (k,j)    */
            idx_t M = iMax - iMin + 1;            /* Number of i values            */
            seq_t Mseq = (seq_t)M;

            /* Constant part of contribution from k and j */
            seq_t jk_value = (seq_t)k * ((k & 1) ? 1 : 0)
                           + (seq_t)j * ((j & 1) ? 1 : 0);
            seq_t jk_contrib = jk_value * Mseq;

            /* Prepare sums needed for the i‑loop */
            seq_t sumI = ((seq_t)iMin + (seq_t)iMax) * Mseq / 2;

            /* Find even i values in [iMin, iMax] */
            idx_t firstEven = (iMin % 2 == 0) ? iMin : iMin + 1;
            idx_t lastEven  = (iMax % 2 == 0) ? iMax : iMax - 1;
            idx_t evenCnt = 0;
            seq_t sumEvenI = 0;
            if (firstEven <= iMax) {
                evenCnt = (lastEven - firstEven) / 2 + 1;
                seq_t firstEvenSeq = (seq_t)firstEven;
                seq_t lastEvenSeq  = (seq_t)lastEven;
                seq_t evenCntSeq   = (seq_t)evenCnt;
                sumEvenI = (firstEvenSeq + lastEvenSeq) * evenCntSeq / 2;
            }

            seq_t sumOddI = sumI - sumEvenI;
            seq_t evenCntSeq = (seq_t)evenCnt;
            seq_t oddCntSeq  = Mseq - evenCntSeq;
            seq_t Cseq = (seq_t)C;

            seq_t inner = 0;
            if (C & 1) { /* C odd: even i contribute via l, odd i via i */
                seq_t sumEvenL = evenCntSeq * Cseq - sumEvenI;
                inner = sumOddI + sumEvenL;
            } else { /* C even: odd i contribute via both i and l */
                seq_t sumOddL = oddCntSeq * Cseq - sumOddI;
                inner = sumOddI + sumOddL;
            }

            total += jk_contrib + inner;
        }
    }

    return total;
}
