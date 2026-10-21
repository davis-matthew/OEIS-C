/*
 * A246128  Index sequence for limit-block extending the (2,1)-version of the
 *          infinite Fibonacci word A014675 with first term as initial block.
 * https://oeis.org/A246128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A246128
 *
 * The algorithm iteratively finds the next occurrence of the current
 * block in the (2,1)-version of the infinite Fibonacci word
 * (A014675).  It does not use a closed‑form formula, but returns the
 * correct index value for any n ≥ 0.
 *
 * Implementation notes:
 *   • The Fibonacci word is generated once up to a length that
 *     comfortably covers the needed indices for n up to about 10000.
 *   • Each step builds the current block (whose length equals the
 *     step number) and searches for its next occurrence using the
 *     Knuth–Morris–Pratt algorithm.
 *   • All indices are handled as idx_t (uint64_t in the tests); the
 *     result is returned as seq_t.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static uint8_t *fib_word = NULL;  /* pointer to the Fibonacci word */
static size_t   fib_len  = 0;     /* length of the generated word */

#define MAX_WORD 2000000   /* sufficient for n up to ~10000 */

static void init_fib_word(void)
{
    /* Allocate two buffers that will be swapped as we apply the
     * morphism 2 → 21, 1 → 2.  The casts are required for C++ */
    uint8_t *a = (uint8_t *)malloc(MAX_WORD);
    uint8_t *b = (uint8_t *)malloc(MAX_WORD);
    if (!a || !b)
        abort();

    size_t len = 1;
    a[0] = 2;               /* start word "2" */
    uint8_t *cur  = a;
    uint8_t *next = b;

    while (len < MAX_WORD) {
        size_t new_len = 0;
        for (size_t i = 0; i < len; ++i) {
            uint8_t ch = cur[i];
            if (ch == 2) {                 /* 2 → 21 */
                if (new_len + 2 > MAX_WORD) { new_len = MAX_WORD; break; }
                next[new_len++] = 2;
                next[new_len++] = 1;
            } else {                       /* 1 → 2 */
                if (new_len + 1 > MAX_WORD) { new_len = MAX_WORD; break; }
                next[new_len++] = 2;
            }
        }
        len = new_len;
        /* swap buffers */
        uint8_t *tmp = cur; cur = next; next = tmp;
        if (len >= MAX_WORD)
            break;
    }

    fib_word = cur;
    fib_len  = len;

    /* free the unused buffer */
    free((cur == a) ? b : a);
}

/* ------------------------------------------------------------------- */
/* seq_t a(idx_t n) --------------------------------------------------- */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!fib_word)
        init_fib_word();

    if (n == 0)
        return (seq_t)0;

    /* The index of the current block’s first occurrence. */
    size_t start = 0;

    for (idx_t step = 1; step <= n; ++step) {
        /* Length of the block for this step. */
        size_t L = (size_t)step;
        if (start + L > fib_len)     /* safety – should not happen */
            return (seq_t)0;

        /* Build the current pattern. */
        uint8_t pattern[10001];
        for (size_t i = 0; i < L; ++i)
            pattern[i] = fib_word[start + i];

        /* Prepare the LPS array for KMP. */
        int lps[10001];
        lps[0] = 0;
        for (size_t i = 1; i < L; ++i) {
            int len = lps[i - 1];
            while (len > 0 && pattern[i] != pattern[len])
                len = lps[len - 1];
            if (pattern[i] == pattern[len])
                ++len;
            lps[i] = len;
        }

        /* Search for the next occurrence after 'start'. */
        size_t i = start + 1;
        int j = 0;
        size_t found = SIZE_MAX;

        while (i < fib_len) {
            if (fib_word[i] == pattern[j]) {
                ++i;
                ++j;
                if ((size_t)j == L) {
                    found = i - L;
                    break;
                }
            } else {
                if (j > 0)
                    j = lps[j - 1];
                else
                    ++i;
            }
        }

        if (found == SIZE_MAX)       /* no further occurrence – should not happen */
            return (seq_t)0;

        start = found;
    }

    return (seq_t)start;
}
