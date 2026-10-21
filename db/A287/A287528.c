/*
 * A287528  Start with 0 and repeatedly substitute 0->012, 1->102, 2->210.
 * https://oeis.org/A287528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nᵗʰ symbol (1‑based) of the fixed point of the
 *            morphism 0 → 012, 1 → 102, 2 → 210 starting with 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t, if needed */
#include <stdint.h>          /* for uint8_t, if needed */

/* The morphism images, indexed by the symbol and the local position 0,1,2. */
static const seq_t MORPH[3][3] = {
    { (seq_t)0, (seq_t)1, (seq_t)2 },   /* 0 → 012 */
    { (seq_t)1, (seq_t)0, (seq_t)2 },   /* 1 → 102 */
    { (seq_t)2, (seq_t)1, (seq_t)0 }    /* 2 → 210 */
};

/* Return the n-th term of sequence A287528.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* Do nothing special for n == 0; the problem guarantees n ≥ 1. */
    idx_t i = n - 1;                /* index in 0‑based form */
    /* Stack (array) to store the remainders while dividing by 3.   */
    idx_t stack[64];
    idx_t top = 0;

    while (i > 0) {
        stack[top++] = i % 3;
        i /= 3;
    }

    /* Start from the base symbol 0 and apply the morphism in reverse
       order (from the shallowest level to the deepest). */
    unsigned int cur = 0;           /* cur ∈ {0,1,2} */
    while (top--) {
        cur = (unsigned int)MORPH[cur][stack[top]];
    }

    return (seq_t)cur;
}
