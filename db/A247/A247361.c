/*
 * A247361  Interleave A000861\{0} and A247359: positive numbers with English
 *          name ending in a vowel resp. consonant.
 * https://oeis.org/A247361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = interleaving of the positive integers whose English name
 *            ends in a vowel (odd positions) and those that end in a
 *            consonant (even positions). The function builds the two
 *            sequences lazily upon request
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   append_to, is_vowel_end
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/append_to.h"
#include "../utils/is_vowel_end.h"

/* Dynamic arrays for the two interleaved sequences. */
static seq_t *V = NULL;  /* vowel‑ending numbers */
static size_t v_len = 0, v_cap = 0;
static seq_t *C = NULL;  /* consonant‑ending numbers */
static size_t c_len = 0, c_cap = 0;
static seq_t nextNum = 1;   /* smallest number not yet classified */

/* Ensure that V and C contain at least needV and needC elements,
   respectively, extending them if necessary. */
static void ensure_both(size_t needV, size_t needC)
{
    while (v_len < needV || c_len < needC) {
        if (is_vowel_end(nextNum))
            append_to(&V, &v_len, &v_cap, nextNum);
        else
            append_to(&C, &c_len, &c_cap, nextNum);
        ++nextNum;
    }
}

/* Main function of the sequence. */
seq_t a(idx_t n)
{
    size_t oddIndex  = (size_t)((n + 1) / 2);
    size_t evenIndex = (size_t)(n / 2);

    if (n % 2) {         /* odd position – vowel‑ending numbers */
        ensure_both(oddIndex, evenIndex);
        return V[oddIndex - 1];
    } else {             /* even position – consonant‑ending numbers */
        ensure_both(oddIndex, evenIndex);
        return C[evenIndex - 1];
    }
}
