/*
 * A381608  Nonnegative integers whose ternary expansion does not contain
 *          pairs of 2's only separated by (zero or more) 1's, with offset 0.
 * https://oeis.org/A381608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth non‑negative integer whose ternary expansion
 *            does not contain a pair of 2's that are separated only by 1's
 *            (leading zeros are ignored). The function a(n) returns the
 *            (n+1)th element of the OEIS sequence A381608, with a(0)=0
 * Range:     u8 n <= 155, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the offset 0 special case. */
    if (n == 0)
        return 0;

    /* The index among the positive (non‑zero) terms. */
    idx_t k = n - 1;

    /* Pre‑compute the automaton counts once. */
    static bool initialized = false;
    enum { MAXLEN = 2000 };           /* plenty for the required limits */
    static idx_t gS[MAXLEN];          /* from state S for suffix length l */
    static idx_t gP[MAXLEN];          /* from state P for suffix length l */
    static idx_t count_len[MAXLEN];   /* valid numbers with length l (l>=1) */
    static idx_t prefix[MAXLEN];      /* cumulative count up to length l */

    if (!initialized) {
        gS[0] = 1;
        gP[0] = 1;
        count_len[0] = 0;
        prefix[0] = 0;
        for (int l = 1; l < MAXLEN; ++l) {
            gS[l] = 2 * gS[l-1] + gP[l-1];
            gP[l] = gS[l-1] + gP[l-1];
            count_len[l] = gS[l-1] + gP[l-1];
            prefix[l] = prefix[l-1] + count_len[l];
        }
        initialized = true;
    }

    /* Locate the length L of the desired number. */
    idx_t L = 1;
    while (L < MAXLEN && prefix[L] <= k)
        ++L;

    /* Offset within the block of numbers of length L. */
    idx_t m = k - prefix[L-1];

    /* Construct the number digit by digit. */
    seq_t result = 0;
    int state = 0; /* 0 = safe (S), 1 = pending (P) */

    for (idx_t pos = 1; pos <= L; ++pos) {
        idx_t rem = L - pos;                 /* remaining digits after this one */
        for (int d = 0; d <= 2; ++d) {
            if (pos == 1 && d == 0)
                continue;                   /* no leading zero except for 0 itself */
            if (state == 1 && d == 2)
                continue;                   /* forbidden transition 2 after 1…1 */

            int next_state = (d == 2) ? 1 : 0;
            idx_t count = (next_state == 0) ? gS[rem] : gP[rem];

            if (count <= m) {
                m -= count;                 /* skip this block */
                continue;
            }

            result = result * (seq_t)3 + (seq_t)d;
            state = next_state;
            break;
        }
    }

    return result;
}
