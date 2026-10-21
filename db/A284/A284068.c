/*
 * A284068  Numbers whose smallest decimal digit is 7.
 * https://oeis.org/A284068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A284068: numbers whose smallest decimal digit is 7.
   This algorithm reconstructs the n‑th member by combinatorial
   counting over the digit alphabet {7,8,9} with the constraint that
   at least one digit is 7. */
#include <stdbool.h>

static seq_t pow3[128];
static seq_t pow2[128];
static bool pow_init = false;
static idx_t pow_initialized = 0;

/* ensure powers of 2 and 3 up to the required length are computed */
static void ensure_powers(idx_t needed)
{
    if (!pow_init) {
        pow3[0] = (seq_t)1;
        pow2[0] = (seq_t)1;
        pow_initialized = 0;
        pow_init = true;
    }
    while (pow_initialized < needed) {
        ++pow_initialized;
        pow3[pow_initialized] = pow3[pow_initialized - 1] * ((seq_t)3);
        pow2[pow_initialized] = pow2[pow_initialized - 1] * ((seq_t)2);
    }
}

seq_t a(idx_t n)
{
    /* Find the length L of the n-th number.  Count of L‑digit
       numbers with digits 7,8,9 and containing at least one 7 is
       3^L - 2^L. */
    idx_t len = 1;
    seq_t cumulative = (seq_t)0;
    while (true) {
        ensure_powers(len);
        seq_t count_len = pow3[len] - pow2[len];
        if (cumulative + count_len >= n)
            break;
        cumulative += count_len;
        ++len;
    }

    /* Offset within the L‑digit block (1‑based). */
    seq_t offset = (seq_t)n - cumulative;

    /* Construct the number digit by digit. */
    seq_t result = (seq_t)0;
    bool has7 = false;
    for (idx_t pos = 0; pos < len; ++pos) {
        idx_t rem = len - pos - 1;
        for (seq_t d = 7; d <= 9; ++d) {
            bool new_has7 = has7 || (d == 7);
            seq_t cnt;
            if (new_has7) {
                cnt = pow3[rem];                     /* any suffix */
            } else {
                cnt = pow3[rem] - pow2[rem];         /* suffix must contain a 7 */
            }
            if (offset > cnt) {
                offset -= cnt;                       /* skip this block */
                continue;
            }
            /* Choose this digit. */
            result = result * ((seq_t)10) + d;
            has7 = new_has7;
            break;
        }
    }
    return result;
}
