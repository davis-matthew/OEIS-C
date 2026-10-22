/*
 * A357137  Maximal run-length of the n-th composition in standard order; a(0)
 *          = 0.
 * https://oeis.org/A357137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximal run-length of the n‑th composition in standard
 *            order
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Determine the sum S for which the n-th composition resides. */
    idx_t cum = 1;         /* counts the compositions before sum S (sum 0 gives 1). */
    idx_t s = 1;
    while (cum + ((idx_t)1 << (unsigned)(s - 1)) <= n) {
        cum += ((idx_t)1 << (unsigned)(s - 1));
        ++s;
    }

    idx_t pos = n - cum;   /* 0‑based index within the compositions of sum s. */

    /* Build the composition parts iteratively.   */
    idx_t parts[64];
    idx_t len = 0;
    idx_t sum = s;
    idx_t index = pos;

    while (sum > 0) {
        for (idx_t f = sum; f > 0; --f) {
            idx_t r = sum - f;
            idx_t count_rem = (r == 0) ? 1 : ((idx_t)1 << (unsigned)(r - 1));
            if (index >= count_rem) {
                index -= count_rem;
                continue;
            }
            parts[len++] = f;
            sum = r;
            break;
        }
    }

    /* Compute the maximal run-length from the parts. */
    seq_t max_run = 1;
    seq_t cur_run = 1;
    idx_t prev = parts[0];

    for (idx_t i = 1; i < len; ++i) {
        if (parts[i] == prev) {
            ++cur_run;
        } else {
            cur_run = 1;
            prev = parts[i];
        }
        if (cur_run > max_run) {
            max_run = cur_run;
        }
    }

    return max_run;
}
