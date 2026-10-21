/*
 * A277745  Trajectory of 1 under repeated application of the morphism 1 ->
 *          1232, 2 -> 1232232, 3 -> 123232.
 * https://oeis.org/A277745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth symbol of the infinite word defined by * the
 *            morphism: 1 → 1232, 2 → 1232232, 3 → 123232. * * The
 *            implementation precomputes the lengths of expansions up to the *
 *            required depth and then descends the expansion tree to determine
 *            the * symbol at position n (1‑based). It works for an...
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* maximum depth required for the problem sizes considered */
    #define MAX_DEPTH 64

    /* lookup table: len[depth][letter] = length of word obtained from
     * a single letter after 'depth' iterations of the morphism.
     * The table is built incrementally and kept across calls. */
    static idx_t len[MAX_DEPTH][4] = {0};
    static int initialized = 0;   /* 0: first call, 1: ready */
    static int currentDepth = 0;   /* depth that covers the largest n seen */

    /* mapping strings for the morphism */
    static const char *const morph[4] = {
        "",           /* 0th element unused */
        "1232",       /* 1 → 1232 */
        "1232232",    /* 2 → 1232232 */
        "123232"      /* 3 → 123232 */
    };

    /* initialise base case for depth 0: a single letter has length 1 */
    if (!initialized) {
        for (int l = 1; l <= 3; ++l) {
            len[0][l] = 1;
        }
        initialized = 1;
    }

    /* grow the depth until the expansion of the initial symbol '1' is
       long enough to contain position n */
    while (len[currentDepth][1] < n) {
        if (currentDepth + 1 >= MAX_DEPTH) {
            /* safety guard, should never be triggered for n ≤ 10000 */
            break;
        }
        ++currentDepth;
        for (int l = 1; l <= 3; ++l) {
            idx_t sum = 0;
            const char *p = morph[l];
            while (*p) {
                int s = *p - '0';
                sum += len[currentDepth - 1][s];
                ++p;
            }
            len[currentDepth][l] = sum;
        }
    }

    /* descend the expansion tree to find the symbol at position n */
    int currentLetter = 1;   /* start with the initial letter '1' */
    int depth = currentDepth;
    idx_t pos = n;           /* 1‑based position within the current block */

    while (depth > 0) {
        const char *p = morph[currentLetter];
        while (*p) {
            int s = *p - '0';
            idx_t blockLen = len[depth - 1][s];
            if (pos > blockLen) {
                pos -= blockLen;   /* skip this block */
            } else {
                /* desired symbol is inside this block */
                currentLetter = s;
                --depth;
                break;
            }
            ++p;
        }
    }

    return (seq_t)currentLetter;
}
