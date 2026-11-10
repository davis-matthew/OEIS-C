/*
 * A260434  Permutation of natural numbers: a(1) = 1, a(2n) = A257803(1+a(n)),
 *          a(2n+1) = A257804(a(n)), where A257803 and A257804 give the
 *          positions of odd and even terms in A233271, the infinite trunk of
 *          inverted binary beanstalk.
 * https://oeis.org/A260434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   a(1) = 1, a(2n) = A257803(1+a(n)), a(2n+1) = A257804(a(n));
 *            computed by walking the bits of n from the top, using positions
 *            of odd/even terms of the trunk A233271
 * Range:     u8 n <= 15, u16 n <= 4610, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 95 terms through n = 9977 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t *pos_odd = NULL, *pos_even = NULL;
static size_t n_odd = 0, n_even = 0, cap_odd = 0, cap_even = 0;
static seq_t trunk_x;          /* last generated trunk term */
static idx_t trunk_i = 0;      /* its index (1-based) */
static int trunk_init = 0;

static void push_pos(idx_t **arr, size_t *cnt, size_t *cap, idx_t v)
{
    if (*cnt >= *cap) {
        size_t nc = *cap ? *cap * 2 : 1024;
        *arr = (idx_t *)realloc(*arr, nc * sizeof(idx_t));
        *cap = nc;
    }
    (*arr)[(*cnt)++] = v;
}

static void trunk_step(void)
{
    if (!trunk_init) {
        trunk_x = 1;
        trunk_i = 1;
        trunk_init = 1;
    } else {
        seq_t t = trunk_x;
        unsigned z = 0;
        while (t > 0) {
            if ((t & 1) == 0) z++;
            t >>= 1;
        }
        trunk_x = trunk_x + 1 + z;
        trunk_i++;
    }
    if ((trunk_x & 1) != 0)
        push_pos(&pos_odd, &n_odd, &cap_odd, trunk_i);
    else
        push_pos(&pos_even, &n_even, &cap_even, trunk_i);
}

/* k-th odd term position (k>=1) */
static idx_t odd_position(idx_t k)
{
    while (n_odd < k) trunk_step();
    return pos_odd[k - 1];
}

/* k-th even term position (k>=1) */
static idx_t even_position(idx_t k)
{
    while (n_even < k) trunk_step();
    return pos_even[k - 1];
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    int top = 63;
    while (((n >> top) & 1) == 0) top--;
    idx_t v = 1;
    for (int b = top - 1; b >= 0; b--) {
        if (((n >> b) & 1) == 0)
            v = odd_position(v + 1);
        else
            v = even_position(v);
    }
    return (seq_t)v;
}
