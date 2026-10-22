/*
 * A380248  The order of the 13 cards of one suit such that after the
 *          SpellUnder-Down deal the cards are in order; a(n) is the n-th card
 *          in the deck.
 * https://oeis.org/A380248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, word
 * Formula:   a(n) is the n‑th card in the deck that yields the ascending deal
 *            [1,2,3,…,13] by the SpellUnder‑Down trick described in A380248.
 *            The deck is computed by reversing the deal steps
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   card_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm
   1. Start with the empty deck (after all cards are dealt).
   2. For i from 13 down to 1:
        - Let m   = size_of_deck + 1
        - Let L   = number of letters of the name of card i
        - Build an array C of length m with C[0] = i and C[1…m‑1] =
          the current deck.
        - Rotate C to the right by L (mod m).  The result is the deck
          before dealing card i.
   3. The final deck gives a(1)…a(13); for n>13 the deck repeats
      periodically. */

#include <stdbool.h>
#include "../utils/card_len.h"

seq_t a(idx_t n) {
    static seq_t deck[13];
    static bool ready = false;
    if (!ready) {
        /* Reverse the SpellUnder‑Down deal to recover the original deck. */
        seq_t after[13];
        idx_t size_after = 0;   /* current number of cards after processed steps */

        for (idx_t i = 13; i >= 1; --i) {
            idx_t m = size_after + 1;      /* size before the current step */
            seq_t C[13];
            C[0] = (seq_t)i;                /* the card that will be dealt */
            /* Copy the current deck to the rest of C. */
            for (idx_t j = 0; j < size_after; ++j)
                C[1 + j] = after[j];

            /* Right‑rotate C by L (mod m) to undo the forward rotation. */
            unsigned int L = card_len(i);
            idx_t shift = L % m;
            for (idx_t j = 0; j < m; ++j) {
                idx_t src = (j + m - shift) % m;   /* (j - shift + m) % m */
                after[j] = C[src];
            }
            size_after = m;
        }

        /* after now contains the original deck. Copy to static array. */
        for (idx_t i = 0; i < 13; ++i)
            deck[i] = after[i];

        ready = true;
    }

    /* The deck repeats every 13 cards. */
    idx_t pos = (n - 1) % 13;
    return deck[pos];
}
