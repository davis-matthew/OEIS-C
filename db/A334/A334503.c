/*
 * A334503  Eventual period of a single cell in rule 131 cellular automaton in
 *          a cyclic universe of width n.
 * https://oeis.org/A334503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = eventual period of a single cell in rule 131 cellular
 *            automaton in a cyclic universe of width n. The value is obtained
 *            by simulating the automaton until a configuration repeats; the
 *            length of that cycle is returned
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stddef.h>

#define MAX_WORDS 200          /* sufficient for n up to 10000 (n/64 <= 157) */
typedef uint64_t word_t;

/* Rotate a bitset of length n left by one position */
static void rotate_left(const word_t *in, word_t *out, size_t words,
                        size_t n, word_t last_mask)
{
    word_t carry = 0;
    for (size_t w = 0; w < words; ++w) {
        word_t next_carry = in[w] >> 63;
        out[w] = (in[w] << 1) | carry;
        carry = next_carry;
    }
    /* wrap the high bit that fell off into bit 0 */
    size_t last_word = (n - 1) / 64;
    int bit_pos = (int)((n - 1) % 64);
    word_t bit_to_wrap = (in[last_word] >> bit_pos) & 1ull;
    out[0] |= bit_to_wrap;
    out[words - 1] &= last_mask;          /* clear unused high bits */
}

/* Rotate a bitset of length n right by one position */
static void rotate_right(const word_t *in, word_t *out, size_t words,
                         size_t n, word_t last_mask)
{
    word_t carry = 0;
    for (int w = (int)words - 1; w >= 0; --w) {
        word_t next_carry = in[w] << 63;
        out[w] = (in[w] >> 1) | carry;
        carry = next_carry;
    }
    /* wrap the low bit that fell off into the last position */
    word_t bit0 = in[0] & 1ull;
    size_t last_word = (n - 1) / 64;
    int bit_pos = (int)((n - 1) % 64);
    out[last_word] |= (bit0 << bit_pos);
    out[words - 1] &= last_mask;          /* clear unused high bits */
}

/* Compute the next state of the automaton according to rule 131 */
static void step_rule131(const word_t *in, word_t *out, size_t words,
                         size_t n, word_t last_mask)
{
    word_t L[MAX_WORDS];
    word_t R[MAX_WORDS];
    rotate_left(in, L, words, n, last_mask);
    rotate_right(in, R, words, n, last_mask);

    for (size_t w = 0; w < words; ++w) {
        word_t C  = in[w];
        word_t T1 = (~L[w]) & (~C);
        word_t T2 = L[w] & C & R[w];
        word_t mask = (w == words - 1) ? last_mask : ~0ull;
        out[w] = (T1 ^ T2) & mask;
    }
}

/* Compare two states for equality */
static int states_equal(const word_t *a, const word_t *b, size_t words,
                        word_t last_mask)
{
    for (size_t w = 0; w < words - 1; ++w) {
        if (a[w] != b[w]) return 0;
    }
    return ((a[words - 1] & last_mask) == (b[words - 1] & last_mask));
}

/* The required function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined, but harmless */

    size_t words = (size_t)((n + 63) / 64);
    size_t rem   = (size_t)(n % 64);
    word_t last_mask = (rem == 0) ? ~0ull : ((1ull << rem) - 1);

    word_t state0[MAX_WORDS]  = {0};
    word_t tortoise[MAX_WORDS]  = {0};
    word_t hare[MAX_WORDS]      = {0};
    word_t tmp[MAX_WORDS]       = {0};
    word_t cur[MAX_WORDS]       = {0};
    word_t nxt[MAX_WORDS]       = {0};

    /* initial configuration: single active cell at position 0 */
    state0[0] = 1ULL;
    memcpy(tortoise, state0, words * sizeof(word_t));
    memcpy(hare, state0, words * sizeof(word_t));

    /* first step for tortoise */
    step_rule131(tortoise, tmp, words, n, last_mask);
    memcpy(tortoise, tmp, words * sizeof(word_t));

    /* first two steps for hare */
    step_rule131(hare, tmp, words, n, last_mask);
    memcpy(hare, tmp, words * sizeof(word_t));
    step_rule131(hare, tmp, words, n, last_mask);
    memcpy(hare, tmp, words * sizeof(word_t));

    /* Floyd's cycle detection */
    while (!states_equal(tortoise, hare, words, last_mask)) {
        step_rule131(tortoise, tmp, words, n, last_mask);
        memcpy(tortoise, tmp, words * sizeof(word_t));

        step_rule131(hare, tmp, words, n, last_mask);
        memcpy(hare, tmp, words * sizeof(word_t));
        step_rule131(hare, tmp, words, n, last_mask);
        memcpy(hare, tmp, words * sizeof(word_t));
    }

    /* Determine the cycle length */
    step_rule131(tortoise, cur, words, n, last_mask);
    size_t period = 1;
    while (!states_equal(tortoise, cur, words, last_mask)) {
        step_rule131(cur, nxt, words, n, last_mask);
        memcpy(cur, nxt, words * sizeof(word_t));
        ++period;
    }

    return (seq_t)period;
}
