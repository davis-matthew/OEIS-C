/*
 * A327613  Number of transfers of marbles between three sets until the first
 *          repetition.
 * https://oeis.org/A327613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A327613: number of transfers until first repetition
   using Floyd's cycle detection. */

#include <stdbool.h>
#include <stddef.h>

/* State of the game: amounts in set A, B, C and the next transfer
   (0 = A→B, 1 = B→C, 2 = C→A). */
struct state_t {
    seq_t a;
    seq_t b;
    seq_t c;
    unsigned char turn;
};

/* Advance one step. */
static inline void next_step(const struct state_t *s, struct state_t *out)
{
    seq_t t;
    switch (s->turn) {
    case 0: /* A -> B */
        t = (s->a + 1) / 2;
        out->a = s->a - t;
        out->b = s->b + t;
        out->c = s->c;
        out->turn = 1;
        break;
    case 1: /* B -> C */
        t = (s->b + 1) / 2;
        out->a = s->a;
        out->b = s->b - t;
        out->c = s->c + t;
        out->turn = 2;
        break;
    case 2: /* C -> A */
        t = (s->c + 1) / 2;
        out->a = s->a + t;
        out->b = s->b;
        out->c = s->c - t;
        out->turn = 0;
        break;
    }
}

/* Equality test. */
static inline bool same_state(const struct state_t *x, const struct state_t *y)
{
    return (x->a == y->a) && (x->b == y->b) &&
           (x->c == y->c) && (x->turn == y->turn);
}

/* Sequence function. */
seq_t a(idx_t n)
{
    /* Initial state: all sets contain n marbles, first transfer is A→B. */
    struct state_t start = { (seq_t)n, (seq_t)n, (seq_t)n, 0 };
    struct state_t tortoise, hare, tmp;

    /* First advance one step for tortoise, two steps for hare. */
    next_step(&start, &tortoise);
    next_step(&tortoise, &hare);

    /* Find meeting point. */
    while (!same_state(&tortoise, &hare)) {
        next_step(&tortoise, &tortoise);
        next_step(&hare, &tmp);
        next_step(&tmp, &hare);
    }

    /* Find mu (length before cycle). */
    struct state_t mu_state = start;
    struct state_t hare_mu = tortoise;
    size_t mu = 0;
    while (!same_state(&mu_state, &hare_mu)) {
        next_step(&mu_state, &mu_state);
        next_step(&hare_mu, &hare_mu);
        ++mu;
    }

    /* Find lambda (cycle length). */
    size_t lambda = 1;
    struct state_t hare_l;
    next_step(&mu_state, &hare_l);
    while (!same_state(&mu_state, &hare_l)) {
        next_step(&hare_l, &hare_l);
        ++lambda;
    }

    /* Total steps until first repetition = mu + lambda. */
    seq_t result = (seq_t)(mu + lambda);
    return result;
}
