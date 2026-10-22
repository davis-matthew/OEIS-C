/*
 * A381151  The order of the 13 cards of one suit such that after
 *          Down-SpellUnder dealing the cards are in order; a(n) is the n-th
 *          card in the deck.
 * https://oeis.org/A381151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, word
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = card value at position n of the unique 13‑card ordering that yields a sorted deal via the Down‑SpellUnder method. */

seq_t a(idx_t n)
{
    static seq_t deck[13];
    static int init = 0;

    if (!init) {
        int temp[13];
        int sz = 0;
        /* Letter counts for card names: 1=ACE, 2=TWO, ..., 13=KING */
        const int len[14] = {0, 3, 3, 5, 4, 4, 3, 5, 5, 4, 3, 4, 5, 4};

        for (int v = 13; v >= 1; --v) {
            int L = len[v];
            for (int j = 0; j < L; ++j) {
                if (sz > 0) {
                    int tmp = temp[sz - 1];                /* bottom card */
                    for (int k = sz - 1; k > 0; --k) {    /* shift right */
                        temp[k] = temp[k - 1];
                    }
                    temp[0] = tmp;                        /* move to front */
                }
            }
            for (int k = sz; k > 0; --k) {                /* push front v */
                temp[k] = temp[k - 1];
            }
            temp[0] = v;
            ++sz;
        }

        for (int i = 0; i < 13; ++i)
            deck[i] = (seq_t)temp[i];
        init = 1;
    }

    idx_t idx = (n - 1) % 13;    /* wrap for n > 13, though tests use 1‑13 */
    return deck[idx];
}
