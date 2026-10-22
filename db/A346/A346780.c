/*
 * A346780  Using the NATO phonetic word for each letter, the last letter of
 *          the word is used for the next word until a repeated word would be
 *          necessary. This sequence shows the number of words for each
 *          letter.
 * https://oeis.org/A346780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, word
 * Formula:   a(n) = length of the chain of distinct NATO phonetic words
 *            reached by repeatedly taking the last letter of the current
 *            word, starting at the ((n-1) mod 26)-th letter of the alphabet
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Mapping from each letter (0 = A, 1 = B, …, 25 = Z) to the index of the
       last letter of its NATO phonetic word. */
    static const int next_letter[26] = {
        0, 14, 4, 0, 14, 19, 5, 11, 0, 19, 14, 0, 4, 17, 17, 0,
        2, 14, 0, 14, 5, 17, 24, 24, 4, 20
    };

    idx_t mod = (n - 1u) % 26u;          /* 0‑based index into alphabet */
    int cur = (int)mod;
    bool visited[26] = { false };

    visited[cur] = true;
    int count = 1;

    while (true) {
        int nxt = next_letter[cur];
        if (visited[nxt])
            break;
        visited[nxt] = true;
        ++count;
        cur = nxt;
    }

    return (seq_t)count;
}
