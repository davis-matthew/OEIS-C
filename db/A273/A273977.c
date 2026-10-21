/*
 * A273977  Words over an alphabet of size 9 that are in standard order with
 *          at least one letter repeated.
 * https://oeis.org/A273977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the nth lexicographically smallest word over the
 *            alphabet {1,…,9} that is in standard order (every letter i>1
 *            appears only after i-1) and contains at least one repeated
 *            letter, represented by concatenating its digits
 * Range:     u8 n <= 5, u16 n <= 70, u32 n <= 9720, u64 n <= 9720,
 *            u128 n <= 9720, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


/* a: compute the nth term of A273977
   n is 1‑based. */
seq_t a(idx_t n)
{
    /* maximum length needed for n up to about 10000 */
    const int MAXLEN = 12;     /* 9 is sufficient, 12 gives buffer */
    const int MAXK   = 9;      /* alphabet size */

    /* table f[len][k] – number of standard words of length 'len'
       with maximum used letter equal to 'k' (k>=1). */
    seq_t f[MAXLEN + 1][MAXK + 1];
    for (int len = 0; len <= MAXLEN; ++len)
        for (int k = 0; k <= MAXK; ++k)
            f[len][k] = (seq_t)0;
    f[1][1] = (seq_t)1;

    for (int len = 2; len <= MAXLEN; ++len) {
        for (int k = 1; k <= MAXK && k <= len; ++k) {
            seq_t term1 = (seq_t)k * f[len - 1][k];
            seq_t term2 = 0;
            if (k > 1) term2 += f[len - 1][k - 1];
            f[len][k] = term1 + term2;
        }
    }

    /* table comp[rem][k] – number of completions of 'rem' remaining
       positions starting from current maximum letter 'k'. */
    seq_t comp[MAXLEN + 1][MAXK + 1];
    for (int k = 1; k <= MAXK; ++k) comp[0][k] = (seq_t)1;
    for (int s = 1; s <= MAXLEN; ++s) {
        for (int k = 1; k <= MAXK; ++k) {
            seq_t val = (seq_t)k * comp[s - 1][k];
            if (k < MAXK) val += comp[s - 1][k + 1];
            comp[s][k] = val;
        }
    }

    /* Find the word length that contains the nth term */
    idx_t cumulative = 0;
    int target_len = 0;
    for (int len = 2; len <= MAXLEN; ++len) {
        idx_t sum = 0;
        int maxk = (len < MAXK ? len : MAXK);
        for (int k = 1; k <= maxk; ++k) sum += (idx_t)f[len][k];
        if (len <= MAXK) sum -= (idx_t)1;   /* exclude the distinct word */
        if (cumulative + sum >= n) {
            target_len = len;
            break;
        }
        cumulative += sum;
    }
    if (target_len == 0) return (seq_t)0;   /* n too large for this precomputation */

    idx_t index = n - cumulative - 1; /* 0‑based index inside words of target_len */

    /* Build the word digit by digit */
    uint8_t digits[MAXLEN];
    digits[0] = 1;          /* first letter is always 1 */
    int k = 1;              /* current maximum letter */
    bool rep = false;       /* whether a repetition has already occurred */

    for (int pos = 2; pos <= target_len; ++pos) {
        int rem = target_len - pos;
        bool chosen = false;

        for (int d = 1; d <= k + 1 && d <= MAXK; ++d) {
            bool new_rep = rep || (d <= k);
            int nk = (d <= k) ? k : k + 1;

            seq_t completions = comp[rem][nk];
            if (target_len <= MAXK && !new_rep && nk == pos) {
                completions -= (seq_t)1;  /* exclude the distinct continuation */
            }

            idx_t comp_idx = (idx_t)completions;
            if (index < comp_idx) {
                digits[pos - 1] = (uint8_t)d;
                k = nk;
                rep = new_rep;
                chosen = true;
                break;
            } else {
                index -= comp_idx;
            }
        }

        if (!chosen) return (seq_t)0;   /* should never happen */
    }

    /* Convert digit array to numeric value */
    seq_t result = (seq_t)0;
    for (int i = 0; i < target_len; ++i) {
        result = result * (seq_t)10 + (seq_t)digits[i];
    }
    return result;
}
