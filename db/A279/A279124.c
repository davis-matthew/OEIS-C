/*
 * A279124  Number of Hangul letters (initials, medials and finals of
 *          syllables) in Sino-Korean name of n. If there are several
 *          different spellings, use the shorter one.
 * https://oeis.org/A279124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, more
 * Formula:   a(n) = number of Hangul jamo letters in the Sino-Korean name of
 *            n (Revised Romanization)
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes this number by decomposing n into decimal groups of four digits
 * (units, thousands, ten-thousands (만), hundred-millions (억), etc.) and summing the
 * jamo counts for each coefficient and suffix. */

static const seq_t coeff_letters[10] = {0, 3, 2, 3, 2, 2, 3, 3, 3, 2};

static seq_t count_block_digits(int val)
{
    seq_t res = 0;
    int d;

    d = val / 1000;                 /* thousands place */
    if (d > 0) {
        if (d != 1) res += coeff_letters[d];
        res += 3;                   /* '천' */
    }
    val %= 1000;

    d = val / 100;                  /* hundreds place */
    if (d > 0) {
        if (d != 1) res += coeff_letters[d];
        res += 3;                   /* '백' */
    }
    val %= 100;

    d = val / 10;                   /* tens place */
    if (d > 0) {
        if (d != 1) res += coeff_letters[d];
        res += 3;                   /* '십' */
    }
    val %= 10;

    d = val;                        /* units place */
    if (d > 0) {
        res += coeff_letters[d];
    }

    return res;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t tmp = n;
    int blockIdx = 0;

    while (tmp > 0) {
        int blockVal = (int)(tmp % 10000);
        tmp /= 10000;
        seq_t blockCount = 0;

        if (blockVal != 0) {
            if (blockIdx == 0) {
                blockCount = count_block_digits(blockVal);
            } else {
                if (blockVal == 1) {
                    blockCount = 2;              /* only the unit word (만, 억, …) */
                } else {
                    blockCount = count_block_digits(blockVal) + 2;
                }
            }
        }
        count += blockCount;
        blockIdx++;
    }

    return count;
}
